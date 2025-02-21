# Character Set과 Collation

## character set
문자들과 그 문자들을 코드화한 인코딩들의 조합으로, MySQL에서는 주로 ASCII, UTF-8, EUC-KR 을 사용한다.

4개의 문자 A, B, a, b가 있다고 가정해보자. 그리고 각 문자에 숫자를 매칭시킨다.; A = 0, B = 1, a = 2, b =3.
이때 A는 symbol이고, 숫자 0은 A를 위한 인코딩(코드화)이다. 그리고 이 4 문자와 인코딩의 조합을 character set이라고 한다.

즉, character set이란 문자 또는 기호들과 그에 매칭되는 인코딩의 집합이다.

## Collation
문자열을 비교, 정렬하기 위해 정의된 규칙들의 집합이다.

각 collation은 하나의 character set에 속해있고, 모든 character set은 적어도 하나 이상의 collation을 갖는다. 

collation은 가중치에 기반하여 나열된다. character set안의 모든 문자는 가중치에 매핑된다. 동일한 가중치를 가지는 문자들을 비교하면 같다고 판단한다. 그리고 서로 다른 가중치를 가지는 문자들을 비교하면, 각 문자가 가지는 가중치에 따라 크고 작음을 결정한다.

위에서 가정한 character set (A = 0, B = 1, a = 2, b =3)에서는 0이 1보다 작기 때문에, A는 B보다 작다. 즉, 이 경우의 collation은‘compare the encodings’ 라는 규칙의 집합이며, 우리는 이를 단순히 binary collation이라 부른다.

이때, 문자 A와 a, B와 b는 동등하다는 규칙을 추가하고 싶다면, '소문자와 대문자 문자는 동등하다'는 규칙을 추가할 수 있다. 그렇게 되면 우리는 A = 0, B = 1, a = 2, b =3, character set에 대하여 2가지의 규칙을 갖게 된다.

---

>  아래 내용은 MySQL 공식문서를 보고 정리한 것으로, MySQL에 제한한다.

테이블 생성 시, Character Set 과 Collation 을 지정할 수 있다. 물론 ALTER 명령어를 사용하여 변경할 수도 있다.
```sql
SELECT DATABASE db_name
	[[DEFULAT] CHARACTER SET charset_name]
	[[DEFULAT] COLLATE collation_name]

```
같은 MySQL 서버에서 다른 character set과 collation을 갖는 database를 생성할 수 있다. 하지만, 조인과 같은 테이블 간 결합 시 테이블의 character set이나 collation이 일치하지 않는다면 쿼리 성능 저하가 올 수 있기 때문에, 동일하게 운영하는 것이 좋다.


### 참고
https://hardlearner.tistory.com/387 <br>
https://dev.mysql.com/doc/refman/8.0/en/charset-general.html <br>
https://dev.mysql.com/doc/refman/8.0/en/adding-collation.html <br>
https://dev.mysql.com/doc/refman/8.4/en/charset-database.html
