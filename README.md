# Monopoly

This project was my Year 12 Software Design and Development project for my major submission.

This code is proper garbage, and I do not recommend following any of this if you are doing your own implementation.

Also the makefile was pre built by the software I was using at the time, and it definitely needs tweaking to compile and run all of this, I could not be bothered to do that though.

Also fyi, The entire codebase is around ***7000 lines***, good luck.

Some fantastic highlights of my coding ability at the time is the `primeRead()` function in `main.cpp`. This stupid thing opens every text file in the `txts/` folder and performs the same action on each of them, checking if there are 40 lines in the file, and if so continuing to the next file. Here is one of the many, many copies of the code blocks in the function: 

```spaceCount = 0;
	while(getline(propertyColourFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking Colour" << endl;
	spaceCountCheck(spaceCount);
  ```
  
This exact same code (except the filename) is used something like **13 times**... With whitespace it takes a total of **~150 lines**...
  
For some reason, I also included a bunch of the ascii art in the `main.cpp` file, including each dice combination, every property card, chance, etc.. The `rollDice()` function for example, takes up **~200 lines** with all the ascii art.
  
In addition to dice combinations, **every single** chance card, **AND** community chest card, **AND** every single property card... Take a guess at just how many lines are taken up by just ascii output? I guarantee it's more than you think.

**1600 lines** from line 1753 to line 3356 is entirely ascii functions that output to the console.

Not to mention the dice rolls, which take an additional 200 lines. Plus a few more spots here and there throughout the code, we'd be looking at around **2000 line** of ascii. ***YIKES***

I haven't looked at the class files, but I'm pretty sure I included a getter and setter for every single variable, whether it was used or not.

Feel free to look at this garbage and laugh, plenty of code to laugh at.
