#!/usr/bin/perl -T
use strict; 
use warnings qw(FATAL all);

$ENV{"PATH"} = "/usr/bin";
delete @ENV{'IFS', 'CDPATH', 'ENV', 'BASH_ENV'};

my ($pid, $uid, $gid, @load, $time, $startTime);

$pid = $$;
$uid = $<;
$gid = (split(/ /, $())[0];

$startTime = time();
$time = 0;

$SIG{HUP} = sub { 
        print("PID: ", $pid); 
};
$SIG{TERM} = sub{ 
        print("GID: ", $gid); 
};
$SIG{INT} = sub { 
        print("UID: ", $uid); 
};
$SIG{USR1} = sub { 
        print("TIME: ", $time); 
};
$SIG{USR2} = sub { 
        printf("LOAD: %s (1 minute)\t %s (5 minutes)\t %s (15 minutes)\n", $load[0], $load[1], $load[2]); 
};

do {
        $time = time() - $startTime;
    @load = split(/load average: /, qx(uptime));
    @load = split(/, /, $load[1]);

        sleep 1;
} while (1);
