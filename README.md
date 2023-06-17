# Snake Game

C++ 프로그래밍 언어로 ncurses 라이브러리를 사용하여 Snake Game을 구현한다.

## 목차
 - [Snake Game의 구조](#snake-game의-구조)
 - [Snake Game의 기본 규칙](#snake-game의-기본적인-규칙)
   - [Rule 1](#rule-1)
   - [Rule 2](#rule-2)
   - [Rule 3](#rule-3)
   - [Rule 4](#rule-4)
   - [Rule 5](#rule-5)
   - [Rule 6](#rule-6)
   - [Custom Rule](#custom-rule)
 - [구현](#구현)
   - [구조](#구조)

---------------
### Snake Game의 구조
    SnakeGame은 기본적으로 다음과 같은 물체들로 구성된다.
    1. Wall(흰색)
        Snake가 통과할 수 없으며 Gate가 생성될 수 있다.
    2. Immuned Wall(흰색에 +)
        Snake가 통과할 수 없으며 Gate 또한 생성될 수 없다.
    3. GrowthItem(파란색)
        Snake가 먹을 시 몸 길이가 한칸 늘어난다.
    4. PoisonItem(빨간색)
        Snake가 먹을 시 몸 길이가 한칸 줄어든다.
    5. Snake Head(옥색)
        Snake의 머리로 진행방향에 따라 한칸 움직인다
    6. Snake Body(초록색)
        Snake의 몸으로 머리가 진행한 곳으로 따라 진행하며 머리와 부딪힐 수 없다.
    7. Gate(자홍색)
        한 쌍씩 나오며 Snake가 한 곳으로 들어가 한 곳으로 나오게 된다.

-------
    
### Snake Game의 기본적인 규칙

#### Rule 1.
  + Snake는 방향키로 조정하며 같은 방향의 방향키는 무시한다.
  + Snake는 진행 방향의 반대 방향으로 움직일 수 없다
  + Snake는 자신의 Body를 통과 할 수 없다.
  + Snake는 Wall을 통과 할 수 없다.
  + Snake는 일정시간(Tick)에 따라 이동한다.
 
#### Rule 2.
  + Snake의 이동방향에 Item이 놓여 있으면 획득한다.
  + Growth Item의 경우 Body의 길이가 1 증가한다.
  + Poison Item의 경우 Body의 길이가 1 감소한다.
  + Body의 길이가 3보다 작아지면 실패한다.
  + 출현 후 일정 시간 후 다른 랜덤한 위치로 이동한다.

#### Rule 3.
  + Gate는 두 개가 한쌍이며 겹치지 않는다.
  + Gate는 임의의 위치에 있는 벽에 나타난다.
  + Gate는 Snake가 진입하면 다른 Gate로 진출한다.
  + Snake가 진입 중에는 사라지지 않으며 다른 위치에 나타나지도 않는다.
  + Gate는 한 번에 한 쌍만 나타난다.
 
#### Rule 4.
  + Gate가 나타나는 벽이 가장자리에 있으면
    - 항상 Map의 안쪽으로 진행한다.
    - 상단 벽 => 아래 방향
    - 하단 벽 => 위 방향
    - 좌측 벽 => 오른쪽 방향
    - 우측 벽 => 왼쪽 방향
  + Gate가 Map의 가운데의 벽에 있으면
    - 다음과 같은 순서로 진출한다.
    - 진입 방향과 일치하는 방향
    - 진입 방향의 시계 방향
    - 진입 방향의 반시계 방향
    - 진입 방향의 반대 방향

#### Rule 5.
  + Wall
    - Gate로 변할 수 있다.
  + Immuned Wall
    - Gate로 변할 수 없다.
  + All Wall
    - Snake Head와 부딪히면 실패한다.

#### Rule 6.
  + 점수 계산
  + 게임 중 최대 길이 계산
    - B: (현재 길이)/(최대 길이)
  + 게임 중 획득한 Growth Item의 수
    - +: (획득한 Growth Item의 수)
  + 게임 중 획득한 Poison Item의 수
    - -: (획득한 Poison Item의 수)
  + 게임 중 통과한 Gate의 수
    - G: (통과한 Gate의 수)
  + 게임 시간
    - time: (시간 second단위)
  + 게임 방법
    - 주어진 미션을 달성하기
  + 미션
    - 길이, Growth Item, Poison Item, Gate를 주어진 양 이상 획득 및 통과하기

#### Custom Rule
  + 각 스테이지에는 랜덤한 위치에 길이 4~10인 2~4개 Map 가운데의 벽이 생성된다.
  + 각 스테이지의 Tick은 50 ~ 300ms이다.
  + Time Item을 획득하면 Tick이 일정 시간 동안 1.5배가 된다.

## 구현

### 구조
 ```bash
 .
 ├── CMakeLists.txt
 ├── README.md
 ├── src
    ├── block.h
    ├── game.h
    ├── main.cpp
    ├── map.h
 ```

 1. `main.cpp` : 게임의 시작화면을 출력한다.
 2. `game.h` : 게임의 진행을 담당한다.
 3. `map.h` : 맵의 저장 및 시작을 담당한다.
 4. `block.h` : 여러 블럭들의 class를 담당한다.