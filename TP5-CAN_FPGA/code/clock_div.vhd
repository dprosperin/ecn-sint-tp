
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity clock_div is
    generic (
            div : positive
            );
    port ( 
            rst : in std_logic; -- reset
            clk_in: in std_logic; -- clock input on FPGA
            clk_out: out std_logic -- clock output 
        );
end clock_div;

architecture Behavioral of clock_div is

begin
    process(clk_in, rst)
        variable cpt: natural := 0;
        begin
            if (rst = '1') then
                cpt := 0;
                clk_out <= '0';
            elsif(rising_edge(clk_in)) then                 
                 if(cpt < div/2) then  
                    clk_out <= '1';
                 else 
                    clk_out <= '0';
                 end if;
                 cpt:= cpt + 1;
                 if(cpt = div) then
                    cpt := 0;
                 end if;
             end if;
        end process;
end Behavioral;
