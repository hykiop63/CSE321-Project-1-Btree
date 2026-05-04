# CSE321-Project-1-Btree

1.실행환경(environment)

Operating System: Windows 11 with WSL2 (Ubuntu 24.04.3LTS)
Implementation Language: C++17,Python 3.12.3
Compiler: GCC 13.3.0

2. run test(csv파일제작)

`make`
`./ass1`

*현재 실험코드는 order가 3에서 5000까지 실행되게 설정되어있습니다. 약 1시간 30분정도 소요됩니다.
실행되는 차수는 main.cpp에 57줄 max_d의 값을 수정하면 변경할 수 있습니다.

*The current experimental code is set to run with an order of 3 to 5000. It takes approximately 1 hour and 30 minutes.
The execution order can be changed by modifying the value of max_d on line 57 of main.cpp.

3. make graph(그래프 제작)

`python3 -m venv venv`
`source venv/bin/activate`
`pip install -r requirements.txt`
`python3 insert_graph.py`
