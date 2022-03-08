# Tic Tac Toe

## 게임 설명
* 3X3 크기의 판에서 상대보다 먼저 가로, 세로 대각선 중 하나 이상의 연속된 3개문양을 만들면 승리한다.

## 사용 방법
* 메뉴에서 1인, 2인 모드 선택 가능.
* 1P가 돌을 선택(O 또는 X)하고 2P는 자동으로 남는 돌 선택.
* 화살표(또는 wsad)와 스페이스바로 원하는 위치에 돌을 놓을 수 있다.

## 코드
#### 클래스
1. Map
    * 3x3 맵
    * **돌 놓기**
    * 게임 종료 체크
2. Player
    * 돌
    * AI 여부
    * Player 정보
    * **돌 놓기**
3. Game
    * 모드
    * 턴
    * 진행
    * **키보드조작**

## 개선 할 사항
1. 돌 놓기 기능 Map과 Player 중복.
2. 키보드 조작 기능 Game에서 별도의 class로 분리하기.
3. UI 개선
4. 키보드 조작 시 기존 위치 출력된 부분 사라지는 문제 해결.
5. AI 랜덤 -> 난이도 추가
```
random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<int> dis(0, 2);
			y = dis(gen);
			x = dis(gen);
```

# 제목1
## 제목2
### 제목3
#### 제목4
##### 제목5
###### 제목6

> 누군가가 뭐라고 말을 했다.
1. 첫번째
1. 두번째
1. 세번째

* 하나
  * 하나하나
* 둘
* 셋

```cpp
printf("dfdf");
```

***

[google](https://google.com)
문장 안에 있는 경우 <https://google.com> 이렇게 넣음.

**강조**

<img src="https://blog.kakaocdn.net/dn/euksHz/btrkikzsvnn/p5AdNUCmxGuZbnu4Vq98Y0/img.webp" width="100px" >



|제목|내용|설명|
|-:|:-:|:-|
|테스트1테스트1테스트1|테스트2|테스트3|
|테스트1|테스트2테스트2테스트2|테스트3테스트3테스트3|

