KeyStore
-- Stores arbitrary values and makes them available for reading

-- Issues:
Serialization of the data make take a long time, particularly if the data being `put` is large. This makes
it hard to be truly persistent.
