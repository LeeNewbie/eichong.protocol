#!/usr/bin/expect
set remote_ip [lindex $argv 0]
set pw [lindex $argv 1]
spawn ssh -l root $remote_ip
expect "password:"
send "$pw\r"
send "service depgate restart\r"
send "exit\r"
expect eof
exit