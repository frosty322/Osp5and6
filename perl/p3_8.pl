#!/usr/bin/perl -T

use strict;
use warnings;

use threads;
use threads::shared;
use Socket;
use IPC::SysV qw(IPC_CREAT IPC_NOWAIT IPC_RMID);

$ENV{"PATH"} = "/usr/bin";

my $pid = $$;
my $uid = $<;
my $gid = $);
my $start_time = time();

my $msg:shared;

my $msqid;

my $uds_thread = threads->create(\&serveUDS);
$uds_thread->detach();

while (1) {
  $msqid = msgget(1488, IPC_CREAT | 0666) or die "$!";

  $msg = pack("l!iiidddd", 1, $pid, $uid, $gid, getAvgLoad(), time() - $start_time);

  msgsnd($msqid, $msg, IPC_NOWAIT) or die "msgsnd: $!";

  sleep 1;

  msgctl($msqid, IPC_RMID, 0) or die "msgctl: $!";
}

sub getAvgLoad {
  my $load_str = `uptime`;
  $load_str =~ s/.*: (.*)/$1/;
  $load_str =~ s/,/\./g;

  return split(/ /, $load_str);
}

sub serveUDS {
  socket(my $listener, AF_INET, SOCK_STREAM, 0) or die "socket: $!";
  my $addr = sockaddr_in("3425", inet_aton("0.0.0.0"));
  bind ($listener, $addr) or die "bind: $!";
  listen ($listener, 1) or die "listen: $!";

  while (1) {
    accept(my $sock, $listener) or die "accept: $!";
    send($sock, $msg, 0) or die "send: $!";
    close $sock or die "close: $!";
  }
}
