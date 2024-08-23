KeyStore
-- Stores arbitrary values and makes them available for reading

-- Assumptions
The calling code will know how to deal with std::any datatypes, including the case where the returned 
`any` value does not actually have a value.
This could be mitigated by adding specific `put()` and `get()` methods for the most common data types.

-- Issues:
Serialization of the data make take a long time, particularly if the data being `put` is large. This makes
it hard to be truly persistent.
