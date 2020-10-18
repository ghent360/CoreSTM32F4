#!/bin/sh
read -r -d '' MEMSCR << EOM
BEGIN {
    FS = " ";
    ram=0;
    ccram=0;
    rom=0;
    other=0;
    romsize = 524288;
    ramsize = 131072;
    ccramsize = 65536;
}
{
    if (\$1>0x20000000) {
        ram+=\$2
    } else if (\$1>=0x10000000 && \$1<0x20000000) {
        ccram+=\$2;
    } else if (\$1>=0x08000000 && \$1<0x10000000) {
        rom+=\$2;
    } else {
        other+=\$2
    }
}
END {
    print "\n";
    print "--------Static Memory Totals-------\n";
    print "Memory     Bytes (dec)\n\n";
    printf("ROM       %6d / %6d (%.1f%%)\n", rom, romsize, rom/romsize * 100);
    printf("RAM       %6d / %6d (%.1f%%)\n", ram, ramsize, ram/ramsize * 100);
    if (ccram > 0) {
      printf("CCRAM     %6d / %6d (%.1f%%)\n", ccram, ccramsize, ccram/ccramsize * 100);
    }
    if (other > 0) {
      printf("Unknown     %d\n", other);
    }
}
EOM
#arm-none-eabi-nm -S --size-sort -s $1 -t d| grep " b \| B " | awk "$MEMSCR"
arm-none-eabi-nm -S --size-sort -s $1 -t d| awk "$MEMSCR"

