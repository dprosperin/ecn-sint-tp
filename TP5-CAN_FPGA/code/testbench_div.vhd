library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity testbench_div is
--  Port ( );
end testbench_div;

architecture Behavioral of testbench_div is
    signal reset : std_logic;
    signal clk : std_logic := '0';
    signal clk_out : std_logic;
begin
    clck_div : entity work.clock_div
        generic map (div => 4)
        port map (
               rst => reset,
               clk_in => clk,
               clk_out => clk_out
               );
    
    processs_reset : process
    begin
        reset <= '1';
        wait for 21 ns;
        reset <= '0';
        wait; 
    end process;

end Behavioral;
