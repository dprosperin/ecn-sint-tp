-- VHDL code for seven-segment display on Basys 3 FPGA
-- Adapted from:
-- https://www.fpga4student.com/2017/09/vhdl-code-for-seven-segment-display.html
-- fpga4student.com: FPGA projects, Verilog projects, VHDL projects
-- Need to add constraints :
--## Clock signal
--set_property PACKAGE_PIN W5 [get_ports clock_100Mhz]							
--	set_property IOSTANDARD LVCMOS33 [get_ports clock_100Mhz]
--	create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports clock_100Mhz]
--set_property PACKAGE_PIN R2 [get_ports reset]     
--        set_property IOSTANDARD LVCMOS33 [get_ports reset]
--##7 segment display
--set_property PACKAGE_PIN W7 [get_ports {led_out[6]}]
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[6]}]
--set_property PACKAGE_PIN W6 [get_ports {led_out[5]}]
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[5]}]
--set_property PACKAGE_PIN U8 [get_ports {led_out[4]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[4]}]
--set_property PACKAGE_PIN V8 [get_ports {led_out[3]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[3]}]
--set_property PACKAGE_PIN U5 [get_ports {led_out[2]}]
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[2]}]
--set_property PACKAGE_PIN V5 [get_ports {led_out[1]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[1]}]
--set_property PACKAGE_PIN U7 [get_ports {led_out[0]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {led_out[0]}]
--set_property PACKAGE_PIN U2 [get_ports {anode_activate[0]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {anode_activate[0]}]
--set_property PACKAGE_PIN U4 [get_ports {anode_activate[1]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {anode_activate[1]}]
--set_property PACKAGE_PIN V4 [get_ports {anode_activate[2]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {anode_activate[2]}]
--set_property PACKAGE_PIN W4 [get_ports {anode_activate[3]}]                    
--    set_property IOSTANDARD LVCMOS33 [get_ports {anode_activate[3]}]


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity display_basys3 is
    Port (
           clock_100Mhz     : in std_logic;-- 100Mhz clock on Basys 3 FPGA board
           reset            : in std_logic; -- reset
           anode_activate   : out std_logic_vector (3 downto 0);-- 4 Anode signals
           led_out          : out std_logic_vector (6 downto 0);-- Cathode patterns of 7-segment display
           displayed_number : in std_logic_vector (15 downto 0) -- Decimal number to be displayed on 4-digit 7-segment display
          );
end display_basys3;

architecture Behavioral of display_basys3 is

    signal led_bcd: std_logic_vector (3 downto 0);
    signal refresh_counter: std_logic_vector (19 downto 0);
    signal led_activating_counter: std_logic_vector(1 downto 0);
    
    -- the other 2-bit for creating 4 LED-activating signals
    -- count         0    ->  1  ->  2  ->  3
    -- activates    LED1    LED2   LED3   LED4
    -- and repeat
    begin
    
    -- Cathode patterns of the 7-segment LED display 
    process(led_bcd)
    begin
        case led_bcd is
            when "0000" => led_out <= "0000001"; -- "0"     
            when "0001" => led_out <= "1001111"; -- "1" 
            when "0010" => led_out <= "0010010"; -- "2" 
            when "0011" => led_out <= "0000110"; -- "3" 
            when "0100" => led_out <= "1001100"; -- "4" 
            when "0101" => led_out <= "0100100"; -- "5" 
            when "0110" => led_out <= "0100000"; -- "6" 
            when "0111" => led_out <= "0001111"; -- "7" 
            when "1000" => led_out <= "0000000"; -- "8"     
            when "1001" => led_out <= "0000100"; -- "9" 
            when "1010" => led_out <= "0000010"; -- a
            when "1011" => led_out <= "1100000"; -- b
            when "1100" => led_out <= "0110001"; -- C
            when "1101" => led_out <= "1000010"; -- d
            when "1110" => led_out <= "0110000"; -- E
            when "1111" => led_out <= "0111000"; -- F
        end case;
    end process;
    
    -- generate refresh period of 10.5ms
    process(clock_100Mhz,reset)
    begin 
        if(reset='1') then
            refresh_counter <= (others => '0');
        elsif(rising_edge(clock_100Mhz)) then
            refresh_counter <= refresh_counter + 1;
        end if;
    end process;
    led_activating_counter <= refresh_counter(19 downto 18);
     
    -- 4-to-1 MUX to generate anode activating signals for 4 LEDs 
    process(led_activating_counter)
    begin
        case led_activating_counter is
        when "00" =>
            anode_activate <= "0111"; 
            led_bcd <= displayed_number(15 downto 12);
        when "01" =>
            anode_activate <= "1011"; 
            led_bcd <= displayed_number(11 downto 8);
        when "10" =>
            anode_activate <= "1101"; 
            led_bcd <= displayed_number(7 downto 4);
        when "11" =>
            anode_activate <= "1110"; 
            led_bcd <= displayed_number(3 downto 0);  
        end case;
    end process;

end Behavioral;