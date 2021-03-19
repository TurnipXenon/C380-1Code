# (A) Protocol

# Visualization
<pre>
                | Observer             User  |
                |  Thread  <->Data<-> Thread |
                |  |   ^              |   |  |
         | <----|--|   |              |   |--|----> |
Observer |      |      |              |      |      | User
         | ---->|------+---> Server <-+------|<---- |
</pre>


# Phase 1: Introduction
When either Observer or User first connects to the server program. They introduce themselves
by sending an enum msg_identity. Based on the enum, the server spawns a thread that will handle everything from now on.

# Phase 2: Initialization

## Observer Client
The observer client will send the string name of the monitored file and the
string ip host via the utility function send_string(). The observer thread spawned by the server will expect to receive two of these strings and receive them via read_string();

## User Client
No exchanges happen between the two after the introduction.

# Phase 3: Routine

## Observer Client
The observer client sends all the inotify events when it detects them. Each inotify event may be
sent as one part or two parts. Usually, the event is sent as a singular struct observer_msg.
If the event has a string name, then, a second part for that one event is sent after.

## Server: Observer Thread
The observer thread spawned in the server will expect the struct observer_msg. It checks the
len property in it, which determines the length of the string sent. If it is greater than zero,
then, it anticipates a string sent by the observer client.

## User Client
The user client sends a success_code to the server, indicating that it is still alive.
Afterwards, it waits for an integer indicating how many entries to expect for printing
on the console. It obtains these string entries via read_string().

## Server: User Thread
The user thread spawned by the server anticipates for a success code, indicating that the client
is still not closing. When it's time to refresh the consoles, the user thread sends how many
entries there are, and sends them in latest to oldest order via send_string() in send_entries().

# Phase 4: Termination

## Clients
Clients would either wait for a kill signal naturally or via an auto kill thread produced, which
raises the kill signal when the client's dormant for at least 60 seconds.

## Server Threads
Server threads are terminated when clients have sent a signal indicating that they got the kill
signal, either via int DISCONNECT_CODE from user client or enum msg_type DISCONNECTION_OBSERVER
from observer client.

## Server
Terminate by sending a kill signal explicitly via command line.

# Utility functions

## send_string()
Sends the length of the string, and then sends the string. It expects that the receiver uses
read_string() to obtain the string.

## read_string()
Anticipates an integer determining the length of the string. Then, it tries to receive this
string. It returns a string allocated in the heap, which puts the responsibility to free onto
its user. Otherwise, it returns a null value when it fails.

# (B) Refinement: Server timestamps