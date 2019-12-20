#!/usr/bin/perl  -T
use warnings;
use strict;
use utf8;

@ENV{PATH, CDPATH, ENV} = qw( /usr/bin );

# Check args
die "usage: $0 <filename>\n" unless @ARGV;

# Open files
open my $file, "<:utf8", $ARGV[0] or die $!;
open my $wc, "| wc" or die $!;

# Slurp in the whole file
$/ = undef;
exit 0 unless defined($_ = <$file>);
# Print every 2nd character
print $wc grep {$/=!$/} split //;

