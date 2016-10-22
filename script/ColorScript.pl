#!/usr/bin/perl

foreach $line (<>) {
   if ($line =~ /^#define/) {
      @partes = split /\s+/, $line;
      @partes[2] =~ s/\,//;
      @partes[3] =~ s/\,//;
      @partes[4] =~ s/\,//;
      print "@partes[0] ";
      print uc "@partes[1] ";
      foreach $x (@partes[2..4]) {
         $x = $x * 255;
         print "$x";
         if ($x ne @partes[4]) {
            print ", ";
         }
      }
      print "\n";
   }
   else {
      print "$line";
   }
}
