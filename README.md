# Basic Database 

This is a simple datastore, written for educational purposes. Below is a list of the database attributes. I hope to increase the number of features and remove limitations over time. Particularly I would like to impliment concurrent reads/writes with locks, and constrates such as FK. However, this is a basic implimentation as it is the first time I have written a data store.

Core Attributes (Features & limitations):
- Single Threaded reads/writes (No locking)
- On disk
- Row database
- Multi table
- 4kb Header and 4kb page size
- Slotted Page Storage
- No variable length columns where each record can allicate only memeory requires.
    - For example a colum of char(255) will use 255 bytes no matter how much is required.#   R e l a t i o n a l D a t a b a s e  
 