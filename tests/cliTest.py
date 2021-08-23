#!/usr/bin/env python3

# This script tests that the compiled binaries produce expected output in
# response to the `--help` option. It also contains a couple of additional
# CLI-related checks as well as test cases for CLI bugfixes.

from testUtils import Utils

import subprocess
import re


def nodeos_help_test():
    help_text = subprocess.check_output(["./programs/nodeos/nodeos", "--help"])
    //use assert
    help_text = subprocess.check_output(["./programs/nodeos/nodeos", "--help"])
    assert(re.search(b'Application.*Options', help_text))
    assert(re.search(b'Options for .*_plugin', help_text))


def cleos_help_test(args):
    help_text = subprocess.check_output(["./programs/cleos/cleos"] + args)
    assert(b'Options:' in help_text)
    assert(b'Subcommands:' in help_text)


def cli11_bugfix_test():
    completed_process = subprocess.run(
        ['./programs/cleos/cleos', '-u', 'http://localhost:0/',
         'push', 'action', 'accout', 'action', '["data"]', '-p', 'wallet'],
        check=False,
        stderr=subprocess.PIPE)

    # The above command must fail because there is no server running
    # on localhost:0
    assert(completed_process.returncode != 0)

    # Make sure that the command failed because of the connection error,
    # not the command line parsing error.
    assert(b'Failed to connect to nodeos' in completed_process.stderr)


try:
    nodeos_help_test()

    cleos_help_test(['--help'])
    cleos_help_test(['system', '--help'])
    cleos_help_test(['version', '--help'])
    cleos_help_test(['wallet', '--help'])

    cli11_bugfix_test()
except Exception as e:
    Utils.errorExit("cli_test: " + str(e))
