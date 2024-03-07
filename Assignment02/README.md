by 

Limitations:
- Server threads that interact with observer clients and user clients don't terminate
  without those client's informing them. I just assume that the clients will always
  successfully send a disconnect code.
- Entries or rows in user entry can only be 1023 characters at most
- File directories or files can only have 1023 characters in their name. Directory
  names near this limit will still have their names truncated in the user client
  due to the previous entry limit.
- Only 256 observer clients can connect simultaneously to a server.
- To prevent a deadlock, I put this mechanism where if a the "traffic enforcer" hasn't
  completely rotated around all the states under two intervals, we open the gates for all
  threads and force them to go back on the first state. Check out output_sorter in server.c.

Known issues (for unknown reasons):
- Sometimes broken pipes happen.
- Often times, the first observer's entries get overwritten by other observers.
