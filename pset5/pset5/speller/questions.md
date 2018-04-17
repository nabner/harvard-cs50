# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

Pneumonoultramicroscopicsilicovolcanoconiosis is a pneumoconiosis (disease of the lungs)
caused by inhalation of very fine silicate or quartz dust.

## According to its man page, what does `getrusage` do?

getrusage() returns resource usage measures for _who_, which can be one
of the following: RUSAGE_SELF, RUSAGE_CHILDREN, or RUSAGE_THREAD.

## Per that same man page, how many members are in a variable of type `struct rusage`?

There are 16 members in a variable of type struct rusage.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

We pass "before" and "after" by reference (instead of by value) to the "calculate" function even though we are not changing their contents because passing a copy of the value would take time and memory and provides no added benefit.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

Main takes in one or two paramenters, one being the file and the
    other being the dictinary by which to check the file's spelling with the
    latter being optional. Main then defines structs for timing the spell check
    process called "before" and "after" Main also defines variables to track the
    time to load, the time to check, the time of size, and the time to unload.
    If two arguments were passed to main, main opens the pointer to argv[1]
    which is the dictionary the user provided.

    Next main loads the dictionary, checking the "loaded" function to make
    sure there were no issues, and calculates the time to load, using the "before"
    and "after" references. Main then creates a pointer called "text" which will
    contain the same number of bits as the file provided by the user, which
    was either argv[1] or argv[2], depending on the value of argc (2 or 3).

    Next, main creates a pointer to a FILE called fp and opens the
    user-provided text file in read mode, as long as fp did not
    point to NULL.

    Before initiating the spell check function, the program prints "MISSPELLED
    WORDS" and initializing 3 variables to keep track of the index, misspellings,
    and number of words. It also defines a char array called "word" that is 46 chars
    long, the max length of a word plus 1 for the null terminator.

    The for loop on line 71 beings the actual spell-checking of words. It defines
    an integer "c" and continues to get the next char from the fp pointer as
    long as c is not at the end of the file.

    The nested if loop on line 74 ensures that the character is either alphabetical or
    the null terminator and that the index is greater than zero. If both of
    those conditions are met, the char is appended to the "word" array and
    index is incremented by one.

    If the index is ever greater than "LENGTH," which was pound-defined
    as 45, we know that the current string is too long to be one of the
    words in our dictionary. If this is the case, we continue to get the
    next char in fp while the chars are alphabetical. Once we reach a non-
    alphabetical number, assuming it's the null terminator, we reset the index
    to 0 and start at line 71 again.

    If the char from the line 71 for loop was not alphabetical, the
    program checks to see if it is a digit. If it is, we continue to get the
    next char in fp while the chars are alphabetical. Once we reach a non-
    alphabetical number, assuming it's the null terminator, we reset the index
    to 0 and start at line 71 again.

    Lastly, if the first two conditions were not met but index is greater than 0,
    the program has found a whole word. Now, set word[index] equal to '\0'
    showing that the word is complete. Next the program will increment the word
    counter, run the mispelled function (tracking the before and after variables in
    order to report the time to check with the time_check calculation on line 116).

    If the word is misspelled, the program prints that word and increments the
    misspellings counter before setting index back to 0 in order to go find
    the next word.

    Line 130 checks to see if there was an error with the fp pointer and if there is,
    it prints an error message and quits the program.

    Afterwards, the program closes the fp FILE. It next calculates the size of the
    dictionary as well as how long it took to calculate the size and then records the
    before and after usage for unloading the file (aka freeing the memory). However,
    if the program was unable to unload, it prints an error message and quits the
    program. If the program was successful in unloading the file, it calculates the
    time_unload.

    Lastly, it reports all of the benchmark variables and returns 0 to indicate
    success.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

Relying on fscanf would not allow the program to check for non-
    alphabetical chars. This would result in spending a lot of time
    checking words against the dictionary that we know are not words
    because they contain non-alpha characters.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

Check and load were declared as constants because they are pointers.
    Any given pointer is only able to point to one memory location at a time
    and the spellcheck program took care of iterating through each of the
    chars after the pointer until a full word was recorded. Because the
    program does not know how long any given word will be, storing check
    and load as constants containing a single pointer will really improve
    the processing speed of the program.
