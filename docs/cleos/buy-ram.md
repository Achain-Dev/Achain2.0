## Overview
This guide provides instructions on how to buy RAM for an Achain blockchain account using the cleos CLI tool. RAM is a system resource used to store blockchain state such as smart contract data and account information.

The example uses `cleos` to buy RAM for the alice account. The alice account pays for the RAM and the alice@active permisssion authorizes the transaction.  

## Before you Begin
Make sure you meet the following requirements: 

* Install the currently supported version of `cleos.`
[[info | Note]]
| `Cleos` is bundled with the Achain software.
* You have access to an Achain blockchain and the `act.system` contract is deployed and used to manage system resources.
* You have an Achain account and access to the account's private key.
* You have sufficient tokens allocated to your account.

## Buy Ram Procedure

The following step shows you how to buy RAM

1. Run the following command to buy RAM worth 0.1 of `ACT` tokens for the alice account:

```shell
cleos system buyram alice alice "0.1 ACT" -p alice@active
```

**Where**
* `alice` = payer, the account paying for RAM.
* `alice` = receiver, the account receiving bought RAM.
* `0.1 ACT` = The amount of tokens to pay for RAM.
* `-p alice@active` = The permission used to authorize the payment, in this case the active permission.  

## Summary
In conclusion, by following these instructions you are able to purchase RAM, with a specified amount of tokens, for the specified accounts.
