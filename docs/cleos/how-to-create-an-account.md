## Goal
Create a new Achain blockchain account 

## Before you begin

* Install the currently supported version of `cleos`

[[info | Note]]
| The cleos tool is bundled with the Achain software.

* Created an Owner and an Active key pair
* Import a private key to the wallet which can authorize on behalf of a creator account

## Steps

**Command**

```sh
cleos create account [creator account name] [new account name][OwnerKey] [ActiveKey]
```
Where:

[creator account name] = name of the existing account that authorizes the creation of a new account

[new account name] = The name of the new account account adhering to Achain account naming conventions

[OwnerKey] = The owner permissions linked to the ownership of the account

[ActiveKey] = The active permissions linked with the creator account

[[info | Recommend]]
| `ActiveKey` is optional but recommended.

[[info | Note]]
| To create a new account in the Achain blockchain, an existing account, also referred to as a creator account, is required to authorize the creation of a new account. For a newly created Achain blockchain, the default system account used to create a new account is act.

**Example Output**
```sh
cleos create account act bob ACT87TQktA5RVse2EguhztfQVEh6XXxBmgkU8b4Y5YnGvtYAoLGNN
executed transaction: 4d65a274de9f809f9926b74c3c54aadc0947020bcfb6dd96043d1bcd9c46604c  200 bytes  166 us
#         act <= act::newaccount            {"creator":"act","name":"bob","owner":{"threshold":1,"keys":[{"key":"ACT87TQktA5RVse2EguhztfQVEh6X...
warning: transaction executed locally, but may not be confirmed by the network yet         ]
```
