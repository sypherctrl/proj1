How to Design a Database in C:
Database: A place in memory where you can store many items, and functions that operateon those items.
Database is built from 3 things:
1. The ENTRY: like a "record" or "row". like when filling out a spreadsheet, try to fill out all the columns for the row.
2. The DATABASE: usually contains
   - a pointer to a dynamically allocated array (of entries)
   - how many entries are currently used
   - total storage capacity
When you design a database you MUST decide where does the storage live?
The DB struct itself dosent store entries. It stores a pointer to the storage of dynamically allocated arrays.
3. Database OPERATIONS: functions that manipulate the entries
