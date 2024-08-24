# KeyStore
Stores arbitrary values and makes them available for reading

## Assumptions
1. The calling code knows what the value type is for a specific key. If this is not the case, then the `std::any` type could
be used and the retrieving code could look at the type of the result. This is not how the current code is implemented.
2. If a value is not available, IE has not been stored, then NAN is returned for `float` and `double`, INT_MIN is returned for integers.
Byte arrays will be null if there is no key value pair stored for the specific key.

## Issues:
1. There may be issues with many threads attempting to read/write due to media performance. The idea here is that
by writing a bunch of small files, the performance will be sufficient compared to updating a large file constantly.
2. Currently there is only one mutex, so all reads/writes are gated by the same restriction. It would be better to have 
this be a lock per file access, to enable concurrent reads/writes.
3. There is currently no implementation for removing a key/value pair. There probably should be.
4. There is currently no generic get that takes a key and tries to find it in the directory structure. There probably should be.
5. The error handling is not very expressive. It should be expanded to better report/log errors
6. There are only minimal tests. 2 threads isn't really adequate for the task. Definitely need a test suite.

## Other
1. This could be done with a transactional database, with the potential for some performance issues  Didn't even think of that at the beginning.
