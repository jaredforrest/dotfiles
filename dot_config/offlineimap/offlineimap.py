#! /usr/bin/env python2
from subprocess import check_output

def get_pass(account):
    return check_output("~/.local/lib/shell/get-pass.sh net/Mailbox", shell=True).splitlines()[0]
