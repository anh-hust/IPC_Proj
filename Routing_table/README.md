## Routing table management project

### Specifications

* Layer 3 routing table
* Maintain routing table, notify any change to connected clients
* Sync up across all clients at any point of time

### Technical Note

1. Route Manager process maintain a routing table
2. The table is managed by Admin
3. Sample table

<br>

| Destination Subnet (Key) | Gateway IP | OIF |
| - | - | - |
| 122.1.1.1/32 | 10.1.1.1 | eth0_0 |
| 123.1.1.1/28 | 123.1.2.3 | eth0_1 |

<br>

4. Operations supported on the table
> * Insert: \<Dest>\<GW>\<OIF>
> * Update: \<Dest>\<new GW>\<new OIF>
> * Delete: \<Dest>

### Working flow
* Whenever User perform any **CUD (CREATE, UPDATE, DELETE)** operation on the routing table, Route Manager Server process sync that particular operation to all connected clients
* When new client connects to the server, Server sends the entire table state to this newly connected client
* At any point of time, Routing table **MUST** be identical on route manager and all connected clients