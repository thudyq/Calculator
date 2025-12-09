# Calculator
	I started this project this summer vacation and finished my first version of programs at the end of July. But later, in order to make other people easier to understand my project and even to continue it, I reconstructed my code from late September, and eventually I finish it today.
	This series of programs serve as a few calculators. Once you enter an equation, the calculator will give you the answer. Their functions seems so simple: lots of physical calculators contain the most function of mine, and even the most complex function of my calculator--the simplification of radical expression--can be solve by AI like Gemini. So my work seems so silly and meaningless. Anyway, I think this is an interesting experience and I have learned a lot from it.
	My initial idea is to develop a calculator that can simplify complex expressions, like (x-2)^2-x^2=-4x+4. However, only the simplification of radical expression part took me lots of time, and I feel it so hard for me to put forward. So I'm not going to put it forward. Welcome all of you to put forward my project to my initial idea (by the way, a new maths learning software called "Liii STEM" has contained this function, and you can download it if you're interested), or to combine it with LateX to make my calculator visible, or just to test my code. Each program's function has been written ahead.
	For the core function in my code, I took an another train of thought. If you have some experience in this part, it's a natural thinking for you to change the infix expression into a postfix expression, using a stack model. But since I have little in that, I adopt the arithmetic pattern we all learn in our primary school--considering the priority of operation. So you may feel confused about my core function--but it works right indeed.
	For the testing dataset, pay attention to their ranges of application which have been listed below.
	Physics_4 ops for integer: test1~5
	Physics_4 ops for decimal: test1~10
	Physics_6 ops: test1~15
	Maths_4 ops for integer: test16~20
	Maths_4 ops for decimal: test16~25
	Maths_6 ops: test26~50
	Maths_6 ops_pro: test26~50
	I know there must be some drawbacks in my project. So if you encounter any problems, please feel free to ask me.
	In the ending, I'd like to express my sincere gratitude to two friends of mine who help me throughout my project, Liu Tianyou(https://github.com/AXeonV) and Hu Junqi(https://github.com/hjq-429). Liu is an excellent programmer who has contributed a lot in Github(he is also a member of the Liii STEM team). He supported me a lot whenever I encounter problems. Hu wrote two programs for me to refer to(cal2.cpp and cal3.cpp). Note that there are differences between his programs' usage and mine. Besides, he use the header file <bits/stdc++.h> that may go wrong in some compilers, so you might need to change it into some other header files. His core function is the pattern I said before, changing the infix expression into a postfix expression. In all, I am extremely grateful to their support. I thought it must be impossible for me to finish the project without their help.
	Thank you for reading,
thudyq
2025.12.9
