## 셋(Set)
데이터의 중복을 허용하지 않는 자료구조.<br> 
수학의 집합과 같은 개념이다.

- Hash 알고리즘을 기반으로 구현되기 때문에, 모든 데이터 작업의 시간 복잡도가 $O(1)$이다. 즉, 다른 자료구조에 비해서 **빠른 속도의 데이터 처리**가 가능하다.
- 해시 셋(Hash Set)이라고도 부른다.

<br>

> List와의 차이점.
> - `List` : **중복된 값과 순서를 보장**하고 싶은 경우
> - `Set` : **중복된 값을 없애고, 순서를 보장하지 않아도** 되는 경우
>

### 추상자료형
```text
- add(data) : 데이터 삽입
- isContain(data) : 데이터 확인. 있으면 true, 없으면 false
- remove(data) : 데이터 제거
- clear() : 셋 비우기
- isEmpty() : 셋이 비었는지 확인
- printAll() : 모든 데이터 출력
```

### 구현 [🔗](https://github.com/yeeuniii/study-algorithm/blob/main/study/data-structure/set/set.py)
- [해시테이블](https://github.com/yeeuniii/study-algorithm/blob/main/study/data-structure/hashtable/hash_table.py)을 이용해서 구현. 
- value 값은 사용하지 않고, key만 사용. 즉, key가 key임과 동시에 value로 사용된다.
- ⚠️ 주의<br>
	HashTable의 `get()` 메서드에서 입력한 key와 일치하는 값이 없는 경우 `None`을 반환하기 때문에, set에 데이터를 추가할 때 value의 기본값은 `None`이 아닌 값으로 초기화해야한다.

<br>

> [그림으로 쉽게 배우는 자료구조와 알고리즘 (기본편)](https://www.inflearn.com/courses/lecture?courseId=328971&type=LECTURE&unitId=115670&subtitleLanguage=ko&tab=curriculum) Section2 Unit11 시청 후 정리
> 

> 추가 참고<br>
> [[자료구조] Set에 대해 알아보자](https://medium.com/@ans188/%EC%9E%90%EB%A3%8C%EA%B5%AC%EC%A1%B0-set%EC%97%90-%EB%8C%80%ED%95%B4-%EC%95%8C%EC%95%84%EB%B3%B4%EC%9E%90-265c32a867fe)
> 