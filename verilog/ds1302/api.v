module ds1302_api(
    input clk,  // clock pulse, whose frequency is 24Mhz
    input rst,  // reset signal
    input write,  // write signal
    input read,  // read signal
    input [4:0] addr,  // ram's five bit address
    input [7:0] byte,  // a byte to be writen into ds1302

    output reg ce,  // select signal, which is valid when ce is high
    output reg sclk,  // communication clock line
    output reg [7:0] led,
    inout [7:0] read_byte  // a byte from ds1302
);

localparam
    sta_idle = 4'h0,  // spare state which is ultra-power consumption
    sta_ce_high = 4'h1,  // ce is high
    sta_ce_low = 4'h2,  // ce is low and stanby
    sta_read = 4'h3,  // read state
    sta_write = 4'h4,  // write state
    sta_write_write_addr = 4'h5,  // write address in a write byte cycle
    sta_write_byte = 4'h6,  // write data state
    sta_write_read_addr = 4'h7,  // write address in a read byte cycle
    sta_read_byte = 4'h8,  // read data state
    sta_transition = 4'h9,  // a temp state in sta_ce_high state transforming into sta_ce_low state
    sta_ending = 4'ha;  // a write or read cycle is ended, then pull-down ce line

reg [3:0] curr_sta;
reg [3:0] next_sta;
reg [7:0] r_byte;  // a byte read from ds1302 if read equals 1
reg [31:0] cnt;
reg led_switch;
reg wr_ack;  // a flag that indicates whether write or read operation done

assign read_byte = read == 1 ? r_byte : {8{1'bz}};  // high impedance if wr is low

always @ * begin
    case (curr_sta)
        sta_idle:
            if (write == 1 || read == 1) next_sta <= sta_ce_high;
            else next_sta <= sta_idle;
        sta_ce_high:
            if (cnt = 32'h00000060) begin  // delay for about 4us
                if (write == 1) next_sta <= sta_write;
                else if (read == 1) next_sta <= sta_read;
                else next_sta <= next_sta <= sta_transition;
            end
            else next_sta <= curr_sta;
        sta_transition:
            if (curr_sta == sta_ce_high) next_sta <= sta_ce_low;
            else next_sta <= sta_ce_high;
        sta_write: next_sta <= sta_write_write_addr;
        sta_write_addr:
            if (wr_ack == 1) next_sta <= sta_write_byte;
            else next_sta <= curr_sta;
        sta_write_byte:
            if (wr_ack == 1) next_sta <= sta_ending;
            else next_sta <= curr_sta;
        sta_read: next_sta <= sta_write_read_addr;
        sta_write_read_addr:
            if (wr_ack == 1) next_sta <= sta_read_byte;
            else next_sta <= curr_sta;
        sta_read_byte:
            if (wr_ack == 1) next_sta <= sta_ending;
            else next_sta <= curr_sta;
        sta_ce_low:
            if (write == 1 || read == 1) next_sta <= sta_transition;
            else if (cnt == 32'h07270e00) next_sta <= sta_idle;  // delay for about 5s. after 5s make this system idle in order to decrease power consumption
            else next_sta <= curr_sta;
        default: next_sta <= sta_idle;
    endcase
end

always @ (posedge clk or negedge rst) begin
    if (!rst) cnt <= 0;
    else if (curr_sta == sta_ce_low || curr_sta == sta_ce_high) cnt <= cnt + 1;
    else cnt <= 0;
end

always @ *
    if (curr_sta == sta_idle) led_switch <= 0;  // if system enters low-power consumption mode close all led in this system
    else led_switch <= 1;  // else open that all led in this system

endmodule
