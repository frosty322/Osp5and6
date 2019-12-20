#!/usr/bin/perl -T
use strict;
use warnings;
use Switch;
use English;


my ($pid, @gid, $uid, @load, $time, $startTime, $sgn);
$startTime = time();
$pid = $PID;
@gid = split(/ /, $GID);
$uid = $UID;

sub up_info{
    $time = time() - $startTime;
    @load = split(/load average: /, qx(uptime));
    @load = split(/, /,$load[1]);
}
sub sig_handler{
    &up_info;
    print("\n");
    switch($sgn){
        case 0 { print("PID: ", $pid); }
        case 1 { print("GID: ", $gid[0]); }
        case 2 { print("UID: ", $uid); }
        case 3 { printf("Server working %d seconds", $time); }
        case 4 { printf("Avg load system time for 1 minute: %s \n\t 5 minutes: %s \n\t 15 minutes: %s ", $load[0], $load[1], $load[2]) } }
    print("\n");

}

$SIG{HUP} = sub{ $sgn=0; &sig_handler; };
$SIG{TERM} = sub{ $sgn=1; &sig_handler; };
$SIG{INT} = sub{ $sgn=2; &sig_handler; };
$SIG{USR1} = sub{ $sgn=3; &sig_handler; };
$SIG{USR2} = sub{ $sgn=4; &sig_handler; };

while (1){
    &up_info;
}
