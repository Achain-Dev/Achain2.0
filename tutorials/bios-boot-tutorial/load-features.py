#!/usr/bin/env python3

import argparse
import json
import numpy
import os
import subprocess
import sys
import time

args = None
logFile = None

#############################################################################################################
## this shell is loading features for Private chain, if you build a private chain, you can run this shell
## cmd: ./load-features.py -l --http-server <specify http server url> --contract-dir <specify contracts build dir> --wallet-url <specify wallet url>
##
## precondition: 
## 1. start nodeos 
## 2. create basic accounts:act.bpay,act.msig,act.names,act.ram,act.ramfee,act.saving,act.stake,act.token,act.vpay,act.rex
## then, run this shell, and then, set system contracts .....
###########################################################################################################
def sleep(t):
    print('sleep', t, '...')
    time.sleep(t)
    print('resume')

def get_default_wallet_url():
    url = 'unix://' + os.environ['HOME'] + '/achain-wallet/keosd.sock'
    return url

def run(args):
    print('load-features.py:', args)
    logFile.write(args + '\n')
    if subprocess.call(args, shell=True):
        print('load-features.py: exiting because of error')
        sys.exit(1)

def getJsonOutput(args):
    print('load-features.py:', args)
    logFile.write(args + '\n')
    proc = subprocess.Popen(args, shell=True, stdout=subprocess.PIPE)
    result = proc.communicate()[0]
    return json.loads(result.decode('utf-8'))

def load_bios_contract():
    run(args.cleos + ' set contract act ' + args.contracts_dir + 'act.bios/ -p act@active')

def load_pre_feature(paras):
    run('curl -X POST ' + args.http_server + '/v1/producer/schedule_protocol_feature_activations -d \'{"protocol_features_to_activate": ["' + paras + '"]}\'')

def load_other_feature(para):
    run(args.cleos + ' push action act activate \'["' + para + '"]\' -p act@active')
 
def loadFeatures():
    cmd=('curl -X POST ' + args.http_server + '/v1/producer/get_supported_protocol_features -d \'{}\'')
    json_result = getJsonOutput(cmd)
    ##print(json_result)
    pre_featue = json_result[0];
    load_pre_feature(pre_featue['feature_digest']);

    sleep(3)
    ##set bios contract
    load_bios_contract();

    ##load other features
    sleep(3)
    i=0
    for each_result in json_result:
        print(each_result['feature_digest'])
        if i==0:
            i=1
            continue
        load_other_feature(each_result['feature_digest'])  
 

# Command Line Arguments
commands = [
    ('l', 'load',           loadFeatures,                True,    "load all features"),
]
parser = argparse.ArgumentParser()

parser.add_argument('--http-server', metavar='', help="http-server-address.i.g:127.0.0.1:8888", default='http://127.0.0.1:8888', dest="http_server")
parser.add_argument('--cleos', metavar='', help="Cleos command", default='../../build/programs/cleos/cleos ')
parser.add_argument('--log-path', metavar='', help="Path to log file", default='./feature.log')
parser.add_argument('--contracts-dir', metavar='', help="Path to contracts file", default='../../build/contracts/' )
parser.add_argument('--wallet-url', metavar='', help="wallet url", default=get_default_wallet_url() )

for (flag, command, function, inAll, help) in commands:
    prefix = ''
    if inAll: prefix += '*'
    if prefix: help = '(' + prefix + ') ' + help
    if flag:
        parser.add_argument('-' + flag, '--' + command, action='store_true', help=help, dest=command)
    else:
        parser.add_argument('--' + command, action='store_true', help=help, dest=command)
        
args = parser.parse_args()


logFile = open(args.log_path, 'a')

logFile.write('\n\n' + '*' * 80 + '\n\n\n')

for (flag, command, function, inAll, help) in commands:
    function()
