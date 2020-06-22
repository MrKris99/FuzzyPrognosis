# FuzzyPrognosis
## Overview
This project uses neuro-fuzzy model, based on user-input linguistics terms and initial COVID-19 spreading trend.
For short term predictions it can work with up to 1% accuracy (depends on user terms quality and, of course, on trend characteristics).

## How to build
It is pure qmake project, with 2 dependencies - jsoncpp and gtest. It is strongly recommended to use conan package manager.
You can use:  
`conan install ./Conan/conanfile.txt`  
and copy result .pri file to FuzzyPrognosis directory. And now you are good to go :)

## Notes for developers
The code is really messy. Yes, I know it. Also there is many memory leaks that came from legacy code. I will fix it at some day.
