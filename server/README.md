# API Server

zTSO uses FreeSO's API architecture for the backend. However, zTSO is not 100% compatible with FreeSO due to differences in how the simulation is managed. Instructions for setting up an API server are available in the appropriate directories. zTSO's API is based on FreeSO `beta-update-88b`.

To maintain compatibility with legacy servers, zTSO follows a slightly altered version scheme that adheres to SemVer guidelines. Instead of `beta/update-88b`, it becomes `beta/update-0.88.101`. Prefixed letters are replaced with a three-digit incremental patch number, akin to .NET's SDK versions. This approach aims to ensure that zTSO clients cannot access FreeSO's servers while remaining familiar to existing users and server operators.

As the API server is already complete, zTSO's client and server will have separate release cycles. The API server will have a Long-Term Support (LTS) release to support existing server operators, while the client will follow a rapid rolling release. To prevent conflicts between the two clients, the API server will be modified to check for zTSO-based clients as part of an additional handshake during login. This check will be ignored by FreeSO-based clients as it constitutes a separate API call.

## Support Cycle

| Version | Release Date | Type | End of Support |
| ------- | ------------ | ---- | -------------- |
| 0.88    | March 31     | LTS  | TBA            |
