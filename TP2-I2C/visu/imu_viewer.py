#!/usr/bin/env python3
"""
Visualisation 3D de l'inclinaison d'une carte à partir d'un accéléromètre (MMA7455L).

Format attendu sur la liaison série (une ligne par échantillon) :
    X, Y, Z\n        ex. : -0.77, -2.00, -0.19

Seuls roulis (roll) et tangage (pitch) sont calculables avec un accéléromètre :
le lacet (yaw, rotation autour de la verticale) ne change pas la gravité mesurée.

Usage :
    python imu_viewer.py                 # premier port série détecté
    python imu_viewer.py -p /dev/ttyUSB0 # Linux
    python imu_viewer.py -p COM3         # Windows
"""

import argparse
import math
import sys
import threading

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import serial
import serial.tools.list_ports


# --------------------------------------------------------------------------
# Lecture série (thread dédié pour ne pas bloquer l'affichage)
# --------------------------------------------------------------------------
class SerialReader(threading.Thread):
    def __init__(self, port, baudrate=9600):
        super().__init__(daemon=True)
        # SERIAL_8N1 : 8 bits de données, pas de parité, 1 bit de stop
        self.ser = serial.Serial(
            port=port,
            baudrate=baudrate,
            bytesize=serial.EIGHTBITS,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            timeout=1,
        )
        self.lock = threading.Lock()
        self.latest = None          # (x, y, z) le plus récent
        self.running = True

    def run(self):
        self.ser.reset_input_buffer()
        while self.running:
            try:
                raw = self.ser.readline()
            except serial.SerialException as e:
                print(f"Erreur série : {e}", file=sys.stderr)
                break
            if not raw:
                continue
            line = raw.decode("ascii", errors="ignore").strip()
            parts = line.split(",")
            if len(parts) != 3:
                continue            # ligne incomplète (ex. au démarrage)
            try:
                x, y, z = (float(p) for p in parts)
            except ValueError:
                continue
            with self.lock:
                self.latest = (x, y, z)

    def get(self):
        with self.lock:
            return self.latest

    def stop(self):
        self.running = False
        self.ser.close()


# --------------------------------------------------------------------------
# Calcul des angles et rotation
# --------------------------------------------------------------------------
def accel_to_angles(x, y, z):
    """Roulis et tangage (rad) à partir du vecteur gravité mesuré."""
    roll = math.atan2(y, z)
    pitch = math.atan2(-x, math.sqrt(y * y + z * z))
    return roll, pitch


def rotation_matrix(roll, pitch):
    cr, sr = math.cos(roll), math.sin(roll)
    cp, sp = math.cos(pitch), math.sin(pitch)
    rx = np.array([[1, 0, 0], [0, cr, -sr], [0, sr, cr]])
    ry = np.array([[cp, 0, sp], [0, 1, 0], [-sp, 0, cp]])
    return ry @ rx


# --------------------------------------------------------------------------
# Géométrie de la carte (pavé plat)
# --------------------------------------------------------------------------
L, W, H = 1.0, 0.6, 0.06   # longueur (X), largeur (Y), épaisseur (Z)
CORNERS = np.array([
    [-L / 2, -W / 2, -H / 2], [L / 2, -W / 2, -H / 2],
    [L / 2,  W / 2, -H / 2], [-L / 2,  W / 2, -H / 2],
    [-L / 2, -W / 2,  H / 2], [L / 2, -W / 2,  H / 2],
    [L / 2,  W / 2,  H / 2], [-L / 2,  W / 2,  H / 2],
])
FACES = [
    [4, 5, 6, 7],  # dessus (composant)
    [0, 1, 2, 3],  # dessous
    [0, 1, 5, 4], [2, 3, 7, 6],
    [1, 2, 6, 5],  # bord +X (avant)
    [0, 3, 7, 4],
]
FACE_COLORS = ["#2e8b57", "#555555", "#999999", "#999999", "#d9534f", "#999999"]


def board_faces(rot):
    pts = CORNERS @ rot.T
    return [[pts[i] for i in face] for face in FACES]


# --------------------------------------------------------------------------
# Programme principal
# --------------------------------------------------------------------------
def pick_port(user_port):
    if user_port:
        return user_port
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        sys.exit("Aucun port série détecté. Précise-le avec -p.")
    print("Ports disponibles :")
    for p in ports:
        print(f"  {p.device} - {p.description}")
    print(f"Utilisation de {ports[0].device}")
    return ports[0].device


def main():
    parser = argparse.ArgumentParser(description="Visualisation 3D accéléromètre")
    parser.add_argument("-p", "--port", help="port série (ex. /dev/ttyUSB0, COM3)")
    parser.add_argument("-b", "--baud", type=int, default=9600)
    parser.add_argument("-a", "--alpha", type=float, default=0.3,
                        help="filtre passe-bas, 0 < alpha <= 1 (1 = pas de filtre)")
    args = parser.parse_args()

    reader = SerialReader(pick_port(args.port), args.baud)
    reader.start()

    fig = plt.figure(figsize=(7, 6))
    ax = fig.add_subplot(projection="3d")
    ax.set_xlim(-0.8, 0.8)
    ax.set_ylim(-0.8, 0.8)
    ax.set_zlim(-0.8, 0.8)
    ax.set_box_aspect((1, 1, 1))
    ax.set_xlabel("X")
    ax.set_ylabel("Y")
    ax.set_zlabel("Z")

    board = Poly3DCollection(board_faces(np.eye(3)), facecolors=FACE_COLORS,
                             edgecolors="k", linewidths=0.5)
    ax.add_collection3d(board)
    title = ax.set_title("En attente de données...")

    state = {"filt": None}

    def update(_frame):
        sample = reader.get()
        if sample is None:
            return board, title
        s = np.array(sample)
        f = state["filt"]
        state["filt"] = s if f is None else args.alpha * s + (1 - args.alpha) * f
        x, y, z = state["filt"]

        roll, pitch = accel_to_angles(x, y, z)
        board.set_verts(board_faces(rotation_matrix(roll, pitch)))
        title.set_text(
            f"Roulis {math.degrees(roll):+6.1f}°   Tangage {math.degrees(pitch):+6.1f}°\n"
            f"X={sample[0]:+.2f}  Y={sample[1]:+.2f}  Z={sample[2]:+.2f}"
        )
        return board, title

    anim = FuncAnimation(fig, update, interval=40, cache_frame_data=False)  # ~25 FPS
    try:
        plt.show()
    finally:
        reader.stop()
    del anim


if __name__ == "__main__":
    main()
