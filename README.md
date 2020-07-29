# Monopoly

This project was my Year 12 Software Design and Development project for my major submission.

This code is proper garbage, and I do not recommend following any of this if you are doing your own implementation.

Some fantastic highlights of my coding ability at the time is the `primeRead()` function in `main.cpp`. This stupid thing opens every text file in the `txts/` folder and performs the same action on each of them, checking if there are 40 lines in the file, and if so continuing to the next file. Here is one of the many, many copies of the code blocks in the function: 

```spaceCount = 0;
	while(getline(propertyColourFile, primer))
	{
		spaceCount++;
	}
//	cout << "checking Colour" << endl;
	spaceCountCheck(spaceCount);
  ```
  
This exact same code (except the filename) is used **13 times**... With whitespace it takes a total of **~150 lines**...
  
For some reason, I also included a bunch of the ascii art in the `main.cpp` file, including each dice combination, every property card, chance, etc.. The `rollDice()` function for example, takes up **~200 lines** with all the ascii art.
  
  
