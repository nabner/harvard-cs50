import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""

        # open a file "positives" in read mode
        # iterate over each line that does not
        # begin with a semicolon
        self.positives_set=set()
        file = open(positives, "r")
        for line in file:
            if line.startswith(";") == False:
                self.positives_set.add(line.rstrip("\n"))
        file.close()
        
        # open a file "negatives" in read mode
        # iterate over each line that does not
        # begin with a semicolon
        self.negatives_set=set()
        file = open(negatives, "r")
        for line in file:
            if line.startswith(";") == False:
                self.negatives_set.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        
        # use a tokenizer to analyze the words in the tweet
        # the tokenizer parses the tweet into individual words
        # and then the analyze function can interate over each word
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        # change all text to lowercase
        # str.lower(tokens)    
    
        # initialize a variable to keep track of the score
        total = 0
    
        # iterate over tokens
        # check if word is positive or negative
        # and increment the score appropriately 
        # if token in positives score += 1 
        for each in tokens:
            word = each.lower()
            if word in self.positives_set:
                total += 1
            elif word in self.negatives_set:
                total -= 1
            else:
                total += 0
        
        # used to debug
        # print(total)
        
        return total

        return 0
