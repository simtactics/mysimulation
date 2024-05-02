# FreeSO API

## Table of Contents  
1. [Overview](#overview)
2. [API](#api)
    1. [Avatars](#avatars)
        1. [Get Avatars By Page](#get-avatars-by-page)
        2. [Get Avatars By Neighborhood](#get-avatars-by-neighborhood)
        3. [Get Online Avatars](#get-online-avatars)
        4. [Get Avatars By IDs](#get-avatars-by-ids)
        5. [Get Avatars By User ID (Protected)](#get-avatars-by-user-id-protected)
    2. [Avatar](#avatar)
        1. [Get Avatar By ID](#get-avatar-by-id)
        2. [Get Avatar By Name](#get-avatar-by-name)
    3. [Lots](#lots)
        1. [Get Lots By Page](#get-lots-by-page)
        2. [Get Lots By Neighborhood](#get-lots-by-neighborhood)
        3. [Get Online Lots](#get-online-lots)
    4. [Lot](#lot)
        1. [Get Lot By ID](#get-lot-by-id)
        2. [Get Lot By Location](#get-lot-by-location)
        3. [Get Lot By Name](#get-lot-by-name)
    5. [Top 100](#top-100)
        1. [Get All Top 100s](#get-all-top-100s)
        2. [Get Top 100 By Category](#get-top-100-by-category)
    6. [Neighborhoods](#neighborhoods)
        1. [Get Neighborhoods](#get-neighborhoods)
    7. [Neighborhood](#neighborhood)
        1. [Get Neighborhood By ID](#get-neighborhood-by-id)
        2. [Get Neighborhood By Name](#get-neighborhood-by-name)
    8. [Bulletins](#bulletins)
        1. [Get Bulletins By Neighborhood](#get-bulletins-by-neighborhood)
        2. [Get Bulletins By Type](#get-bulletins-by-type)
    9. [Bulletin](#bulletin)
        1. [Get Bulletin By ID](#get-bulletin-by-id)
    10. [Election](#election)
        1. [Get Election By Neighborhood](#get-election-by-neighborhood)
    11. [OAuth](#oauth)
        1. [Get Token](#get-token)
    12. [City](#city)
        1. [city.json](#cityjson)
    13. [Media](#media)
        1. [Get Lot Thumbnail](#get-lot-thumbnail)
3. [Extending The API](#extending-the-api)
4. [Notable Projects Using The API](#notable-projects-using-the-api)

# Overview
This document describes the resources that make up FreeSO's public API. It is currently in beta and subject to change at any time.

The live server API, `api.freeso.org` can be accessed via HTTP or HTTPS. You should use HTTPS to access the live server API unless absolutely necessary. This documentation will include examples using the live server, but if you are building and testing new API features in a dev build, just replace `api.freeso.org` with whatever your API URL is (ex. `localhost:9000/userapi/city/1/city.json`).

Feel free to update missing/incorrect information by [editing this page](https://github.com/riperiperi/FreeSO/wiki/Public-API/_edit).

**For more information about some of the data returned by the API, please see [Public API Data Definitions](https://github.com/riperiperi/FreeSO/wiki/Public-API-Data-Definitions)**

***

## Avatars
### Get Avatars By Page
```
/userapi/city/{shardId}/avatars/page/{pageNum}?avatars_on_page={avatarsPerPage}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `pageNum` | `integer` | The page of avatars you'd like to request. pageNum `1` will give you the total number of pages. |
| `avatarsPerPage` | `integer` (Optional) | The number of avatars to return per page. Default is `100`, max is `500`. |

#### Response
```json
{
  "page": 1,
  "total_pages": 1,
  "total_avatars": 1,
  "avatars_on_page": 1,
  "avatars": [
    {
      "avatar_id": 887,
      "shard_id": 1,
      "name": "burglar cop",
      "gender": 0,
      "date": 1486420929,
      "description": "  the law is a suggestion\r\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\r\ndont freak out if im just standing there starving im probably developing something worth dying for\r\n",
      "current_job": 5,
      "mayor_nhood": null
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/avatars/page/1>

### Get Avatars By Neighborhood
```
/userapi/city/{shardId}/avatars/neighborhood/{nhoodId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |

#### Response
```json
{
  "avatars": [
    {
      "avatar_id": 887,
      "shard_id": 1,
      "name": "burglar cop",
      "gender": 0,
      "date": 1486420929,
      "description": "  the law is a suggestion\r\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\r\ndont freak out if im just standing there starving im probably developing something worth dying for\r\n",
      "current_job": 5,
      "mayor_nhood": null
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/avatars/neighborhood/54>

### Get Online Avatars
```
/userapi/avatars/online?compact={compactBool}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `compact` | `boolean` (Optional) | Compact returns `avatars_online_count` and an empty `avatars` array. Default is false. |

#### Response
```json
{
  "avatars_online_count": 1,
  "avatars": [
    {
      "avatar_id": 1,
      "name": "burglar cop",
      "privacy_mode": 0,
      "location": 13828398
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/avatars/online>

### Get Avatars By IDs
```
/userapi/avatars/?ids={avatarIds}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `avatarIds` | `string` | A list of `avatar_id`s from `fso_avatars` separated by commas. |

#### Response
```json
{
  "avatars": [
    {
      "avatar_id": 887,
      "shard_id": 1,
      "name": "burglar cop",
      "gender": 0,
      "date": 1486420929,
      "description": "  the law is a suggestion\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\ndont freak out if im just standing there starving im probably developing something worth dying for\n",
      "current_job": 5,
      "mayor_nhood": null
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/avatars?ids=887,30363>

### Get Avatars By User ID (Protected)
```
/userapi/user/avatars
```
Returns an array of the avatars associated with a User ID. Requires a [valid token](#get-token) for authorization.

#### Response
```json
{
  "avatars": [
    {
      "avatar_id": 887,
      "shard_id": 1,
      "name": "burglar cop",
      "gender": 0,
      "date": 1486420929,
      "description": "  the law is a suggestion\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\ndont freak out if im just standing there starving im probably developing something worth dying for\n",
      "current_job": 5,
      "mayor_nhood": null
    }
  ]
}
```
#### Live Response
Send a GET request using something like [Postman](https://www.getpostman.com/downloads/) with an authorization header of type bearer token and a [valid token](#get-token) to <https://api.freeso.org/userapi/user/avatars>

## Avatar
### Get Avatar By ID
```
/userapi/avatars/{avatarId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `avatarId` | `integer` | The value of `avatar_id` in `fso_avatars` for an avatar. |

#### Response
```json
{
  "avatar_id": 887,
  "shard_id": 1,
  "name": "burglar cop",
  "gender": 0,
  "date": 1486420929,
  "description": "  the law is a suggestion\r\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\r\ndont freak out if im just standing there starving im probably developing something worth dying for\r\n",
  "current_job": 5,
  "mayor_nhood": null
}
```
#### Live Response
<https://api.freeso.org/userapi/avatars/887>

### Get Avatar By Name
```
/userapi/city/{shardId}/avatars/name/{avatarName}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `avatarName` | `string` | The value of `name` in `fso_avatars` for an avatar. **Warning: Make sure this string is URL encoded** |

#### Response
```json
{
  "avatar_id": 887,
  "shard_id": 1,
  "name": "burglar cop",
  "gender": 0,
  "date": 1486420929,
  "description": "  the law is a suggestion\r\n          but its a pretty good one tbh\r\n\r\n                                                     -me\r\n\r\ndont freak out if im just standing there starving im probably developing something worth dying for\r\n",
  "current_job": 5,
  "mayor_nhood": null
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/avatars/name/burglar%20cop>

## Lots
### Get Lots By Page
```
/userapi/city/{shardId}/lots/page/{pageNum}?lots_on_page={lotsPerPage}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `pageNum` | `integer` | The page of lots you'd like to request. pageNum `1` will give you the total number of pages. |
| `lotsPerPage` | `integer` (Optional) | The number of lots to return per page. Default is `100`, max is `500`. |

#### Response
```json
{
  "page": 1,
  "total_pages": 1,
  "total_lots": 1,
  "lots_on_page": 1,
  "lots": [
    {
      "lot_id": 6852,
      "location": 13828398,
      "name": "M.O.M.I. Headquarters",
      "description": "",
      "category": 5,
      "admit_mode": 3,
      "neighborhood_id": 54,
      "avatars_in_lot": 0
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/page/1>

### Get Lots By Neighborhood
```
/userapi/city/{shardId}/lots/neighborhood/{nhoodId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |

#### Response
```json
{
  "lots": [
    {
      "lot_id": 6852,
      "location": 13828398,
      "name": "M.O.M.I. Headquarters",
      "description": "",
      "category": 5,
      "admit_mode": 3,
      "neighborhood_id": 54,
      "avatars_in_lot": 0
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/neighborhood/54>

### Get Online Lots
```
/userapi/city/{shardId}/lots/online?compact={compactBool}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `compactBool` | `boolean` (Optional) | Compact returns `total_lots_online`, `total_avatars_in_lots_online` and an empty `lots` array. Default is false. |

#### Response
```json
{
  "total_lots_online": 1,
  "total_avatars_in_lots_online": 1,
  "lots": [
    {
      "location": 13828398,
      "name": "M.O.M.I. Headquarters",
      "description": "",
      "category": 5,
      "neighborhood_id": 54,
      "avatars_in_lot": 0
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/online>

### Get Lots By IDs
```
/userapi/lots?ids={lotIds}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `lotIds` | `string` | A list of `lot_id`s from `fso_lots` separated by commas. |

#### Response
```json
{
  "lots": [
    {
      "lot_id": 6852,
      "shard_id": 1,
      "owner_id": 887,
      "roommates": [
        887
      ],
      "location": 13828398,
      "name": "M.O.M.I. Headquarters",
      "description": "",
      "location": 13828398,
      "neighborhood_id": 54,
      "created_date": 1518961270,
      "category": 5,
      "skill_mode": 0,
      "admit_mode": 1
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/lots?ids=6852,6634>

## Lot
### Get Lot By ID
```
/userapi/lots/{lotId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `lotId` | `integer` | The value of `lot_id` in `fso_lots` for a lot. |

#### Response
```json
{
  "lot_id": 6852,
  "shard_id": 1,
  "owner_id": 887,
  "roommates": [
    887
  ],
  "name": "M.O.M.I. Headquarters",
  "description": "",
  "location": 13828398,
  "neighborhood_id": 54,
  "created_date": 1518961270,
  "category": 5,
  "skill_mode": 0,
  "admit_mode": 1
}
```
#### Live Response
<https://api.freeso.org/userapi/lots/6852>

### Get Lot By Location
```
/userapi/city/{shardId}/lots/location/{locationId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `locationId` | `integer` | The value of `location` in `fso_lots` for a lot. |

#### Response
```json
{
  "lot_id": 6852,
  "shard_id": 1,
  "owner_id": 887,
  "roommates": [
    887
  ],
  "name": "M.O.M.I. Headquarters",
  "description": "",
  "location": 13828398,
  "neighborhood_id": 54,
  "created_date": 1518961270,
  "category": 5,
  "skill_mode": 0,
  "admit_mode": 1
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/location/13828398>

### Get Lot By Name
```
/userapi/city/{shardId}/lots/name/{lotName}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `lotName` | `string` | The value of `name` in `fso_lots` for a lot. **Warning: Make sure this string is URL encoded** |

#### Response
```json
{
  "lot_id": 6852,
  "shard_id": 1,
  "owner_id": 887,
  "roommates": [
    887
  ],
  "name": "M.O.M.I. Headquarters",
  "description": "",
  "location": 13828398,
  "neighborhood_id": 54,
  "created_date": 1518961270,
  "category": 5,
  "skill_mode": 0,
  "admit_mode": 1
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/name/M.O.M.I.%20Headquarters>

## Top 100
### Get All Top 100s
```
/userapi/city/{shardId}/lots/top100/all
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |

#### Response
```json
{
  "lots": [
    {
      "category": 5,
      "rank": 1,
      "shard_id": 1,
      "lot_name": "M.O.M.I. Headquarters",
      "lot_location": 13828398,
      "lot_id": 6852
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/top100/all>

### Get Top 100 By Category
```
/userapi/city/{shardId}/lots/top100/category/{lotCategory}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `lotCategory` | `integer` | The category type (1-11) |

#### Response
```json
{
  "lots": [
    {
      "category": 5,
      "rank": 1,
      "shard_id": 1,
      "lot_name": "M.O.M.I. Headquarters",
      "lot_location": 13828398,
      "lot_id": 6852
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/lots/top100/category/5>

## Neighborhoods
### Get Neighborhoods
```
/userapi/city/{shardId}/neighborhoods/all
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |

#### Response
```json
{
  "neighborhoods": [
    {
      "neighborhood_id": 54,
      "name": "D.A.M.N.",
      "description": "Fully known as the D.A.D.D.I. And M.O.M.I. Neighborhood. The home of the Sunrise Crater government, who have the final say over all decisions and neighborhoods despite never being elected.\r\n\r\nFamous for its huge annual events, incredibly high rate of arson and non-existent firefighter response, this location has a higher mortality rate for visitors than any other mountain on the planet.",
      "color": 0,
      "town_hall_id": 6634,
      "icon_url": null,
      "mayor_id": 108523,
      "mayor_elected_date": 1565012163,
      "election_cycle_id": null
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/neighborhoods/all>

## Neighborhood
### Get Neighborhood By ID
```
/userapi/neighborhoods/{nhoodId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |

#### Response
```json
{
  "neighborhood_id": 54,
  "name": "D.A.M.N.",
  "description": "Fully known as the D.A.D.D.I. And M.O.M.I. Neighborhood. The home of the Sunrise Crater government, who have the final say over all decisions and neighborhoods despite never being elected.\r\n\r\nFamous for its huge annual events, incredibly high rate of arson and non-existent firefighter response, this location has a higher mortality rate for visitors than any other mountain on the planet.",
  "color": 0,
  "town_hall_id": 6634,
  "icon_url": null,
  "mayor_id": 108523,
  "mayor_elected_date": 1565012163,
  "election_cycle_id": null
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhoods/54>

### Get Neighborhood By Name
```
/userapi/neighborhoods/{nhoodName}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodName` | `string` | The value of `name` in `fso_neighborhoods` for a neighborhood. **Warning: Make sure this string is URL encoded** |

#### Response
```json
{
  "neighborhood_id": 54,
  "name": "D.A.M.N.",
  "description": "Fully known as the D.A.D.D.I. And M.O.M.I. Neighborhood. The home of the Sunrise Crater government, who have the final say over all decisions and neighborhoods despite never being elected.\r\n\r\nFamous for its huge annual events, incredibly high rate of arson and non-existent firefighter response, this location has a higher mortality rate for visitors than any other mountain on the planet.",
  "color": 0,
  "town_hall_id": 6634,
  "icon_url": null,
  "mayor_id": 108523,
  "mayor_elected_date": 1565012163,
  "election_cycle_id": null
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhoods/D.A.M.N.>

## Bulletins
### Get Bulletins By Neighborhood
```
/userapi/neighborhood/{nhoodId}/bulletins
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |

#### Response
```json
{
  "bulletins": [
    {
      "bulletin_id": 241,
      "neighborhood_id": 54,
      "avatar_id": 887,
      "title": "Why are we still here?",
      "body": "Just to Suffer?",
      "date": 1561318161,
      "flags": 0,
      "lot_id": 13828399,
      "type": 0
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhood/54/bulletins>

### Get Bulletins By Type
```
/userapi/neighborhood/{nhoodId}/bulletins/type/{bulletinType}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |
| `bulletinType` | `integer` | `0` - Mayor posts, `1` - System messages, `2` - Community posts. **Warning: System messages may reference game assets instead of providing a human readable bulletin.** |

#### Response
```json
{
  "bulletins": [
    {
      "bulletin_id": 241,
      "neighborhood_id": 54,
      "avatar_id": 887,
      "title": "Why are we still here?",
      "body": "Just to Suffer?",
      "date": 1561318161,
      "flags": 0,
      "lot_id": 13828399,
      "type": 0
    }
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhood/1/bulletins/type/0>

## Bulletin
### Get Bulletin By ID
```
/userapi/neighborhood/{nhoodId}/bulletins/{bulletinId}
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |
| `bulletinId` | `integer` | The value of `bulletin_id` in `fso_bulletin_posts` for a bulletin. |

#### Response
```json
{
  "bulletin_id": 241,
  "neighborhood_id": 54,
  "avatar_id": 887,
  "title": "Why are we still here?",
  "body": "Just to Suffer?",
  "date": 1561318161,
  "flags": 0,
  "lot_id": 13828399,
  "type": 0
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhood/54/bulletins/241>

## Election
### Get Election By Neighborhood
```
/userapi/neighborhood/{nhoodId}/elections
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `nhoodId` | `integer` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |

#### Response
```json
{
  "current_state": 1,
  "neighborhood_id": 54,
  "start_date": 1565630999,
  "end_date": 1566062999,
  "candidates": []
}
```
```json
{
  "error": "Election cycle not found"
}
```
#### Live Response
<https://api.freeso.org/userapi/neighborhood/54/elections>

## OAuth
### Get Token
```
/userapi/oauth/token
```
> Warning: Access to this endpoint may be restricted by URL/IP at any time.

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `username` | `string` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |
| `password` | `string` | The value of `neighborhood_id` in `fso_neighborhoods` for a neighborhood. |
| `permission_level` | `integer` | The level of account access requested for the token (1-4). 1 - Read, 2 - Update, 3 - Write, 4 - Delete |

#### Response
```json
{
  "access_token": "ROoPRE007bqYBABGylS3TJREv8kOh49E6z7Qa2wXBryCxcRn8NFWrV48SNTKVL1K7wkwAEsqeKMHoIi6F4WOkZYUiKZ8ar0II3JqOZB9db3rgfHpZELcYvsCNM0CBy4pIaofhnoj2qI4JVy7mq27i9O0ZiIMLB9JLksxsmMAMefm8482VU5RjhDQRX16jFNjoEK8Rtb3rBJq340LCeAA9XOgnisG4iwjeaK1zjgU93i8VCe1GTpCowS5C9UX2",
  "expires_in": 3600
}
```
#### Live Response:
Send a `form-data` POST containing the parameters using something like [Postman](https://www.getpostman.com/downloads/) to <https://api.freeso.org/userapi/oauth/token>

## City
### city.json
```
/userapi/city/{shardId}/city.json
```
Returns every reserved lot, its name, online lots, and all online lot's populations.

> **Warning:** This endpoint may be deprecated in favor of recent API additions. You should migrate existing projects to use them.

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |

#### Response Description
| Name | Type | Description |
| -- | -- | --------- |
| `names` | `array[string]` | An array of every lot name in alphabetical order. |
| `reservedLots` | `array[integer]` | An array of every reserved lot's location (the index matches up with the names array). |
| `activeLots` | `array[integer]` | An array of lot locations for lots that are open. |
| `onlineCount` | `array[integer]` | An array of lot population for lots that are open (the index matches up with the activeLots array). |

#### Response
```json
{
  "names": [
    "Sunrise Crater Town Hall",
    "M.O.M.I. Headquarters"
  ],
  "reservedLots": [
    13828397,
    13828398
  ],
  "activeLots": [
    13828397
  ],
  "onlineCount": [
    10
  ]
}
```
#### Live Response
<https://api.freeso.org/userapi/city/1/city.json>

## Media
### Get Lot Thumbnail
```
/userapi/city/{shardId}/{lotLocation}.png
```

#### Parameters
| Name | Type | Description |
| ---- | ---- | ----------- |
| `shardId` | `integer` | The value of `shard_id` in `fso_shards` for a city. This will likely always be `1`. |
| `lotLocation` | `integer` | The location of a reserved lot. |

#### Response
![M.O.M.I. Headquarters thumbnail](https://api.freeso.org/userapi/city/1/13828398.png)
#### Live Response
<https://api.freeso.org/userapi/city/1/13828398.png>

***

# Extending The API
If the API is missing some data you think would be beneficial, you can open an [issue](https://github.com/riperiperi/FreeSO/issues) to discuss it, but the best solution would be to submit a [pull request](https://github.com/riperiperi/FreeSO/pulls) yourself. [This PR](https://github.com/riperiperi/FreeSO/pull/152) by [@Cowplant-Simmer-Collin](https://github.com/Cowplant-Simmer-Collin) is a great example of how to add new API features.

# Notable Projects Using The API
The following are popular projects known to consume the API. They should be notified of any breaking changes. Feel free to add your own to this list.
* [TSOMania](http://www.tsomania.net/)
* [FreeSO Dashboard](https://dashboard.thecode.house) [@dotequals](https://github.com/dotequals)
* [FreeSO Notifier](https://discord.gg/KuzzJZ7) - Nebulae#0666