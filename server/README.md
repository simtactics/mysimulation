# API Server

zTSO utilizes the FreeSO API architecture for its backend, albeit not entirely compatible  due to differences in simulation management.

### Versioning Scheme

Instead of using FreeSO's versioning scheme, zTSO adopts a versioning pattern that adheres to SemVer guidelines with some modifications:

- FreeSO version ``beta/update-88b`` becomes ``beta/update-0.88.101`` in zTSO.
- Prefixed letters in FreeSO version are replaced with a three-digit incremental patch number in zTSO, akin to .NET's SDK versions.

This versioning strategy aims to ensure that zTSO servers remain distinct from FreeSO-compatible servers while maintaining familiarity for existing users and server operators.