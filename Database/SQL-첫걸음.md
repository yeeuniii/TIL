# SQL 첫걸음

<!-- TOC -->

- [SQL 첫걸음](#sql-%EC%B2%AB%EA%B1%B8%EC%9D%8C)
	- [[1장 - 데이터베이스와 SQL]](#1%EC%9E%A5---%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4%EC%99%80-sql)
		- [데이터베이스](#%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4)
			- [데이터베이스 관리 시스템DBMS; Database Management System](#%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4-%EA%B4%80%EB%A6%AC-%EC%8B%9C%EC%8A%A4%ED%85%9Cdbms-database-management-system)
			- [SQL](#sql)
		- [다양한 데이터베이스](#%EB%8B%A4%EC%96%91%ED%95%9C-%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4)
			- [관계형 데이터베이스 RDB; Relational Database](#%EA%B4%80%EA%B3%84%ED%98%95-%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4-rdb-relational-database)
			- [데이터베이스 종류](#%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4-%EC%A2%85%EB%A5%98)
			- [RDBMS 종류](#rdbms-%EC%A2%85%EB%A5%98)
		- [데이터베이스 서버](#%EB%8D%B0%EC%9D%B4%ED%84%B0%EB%B2%A0%EC%9D%B4%EC%8A%A4-%EC%84%9C%EB%B2%84)
			- [RDBMS의 클라이언트/서버](#rdbms%EC%9D%98-%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8%EC%84%9C%EB%B2%84)
			- [루프 백 접속](#%EB%A3%A8%ED%94%84-%EB%B0%B1-%EC%A0%91%EC%86%8D)
	- [[2장 - 테이블에서 데이터 검색]](#2%EC%9E%A5---%ED%85%8C%EC%9D%B4%EB%B8%94%EC%97%90%EC%84%9C-%EB%8D%B0%EC%9D%B4%ED%84%B0-%EA%B2%80%EC%83%89)
		- [검색](#%EA%B2%80%EC%83%89)
			- [테이블 구조 참조하기](#%ED%85%8C%EC%9D%B4%EB%B8%94-%EA%B5%AC%EC%A1%B0-%EC%B0%B8%EC%A1%B0%ED%95%98%EA%B8%B0)
			- [자료형](#%EC%9E%90%EB%A3%8C%ED%98%95)
		- [검색 조건 지정하기](#%EA%B2%80%EC%83%89-%EC%A1%B0%EA%B1%B4-%EC%A7%80%EC%A0%95%ED%95%98%EA%B8%B0)
		- [패턴 매칭에 의한 검색](#%ED%8C%A8%ED%84%B4-%EB%A7%A4%EC%B9%AD%EC%97%90-%EC%9D%98%ED%95%9C-%EA%B2%80%EC%83%89)
			- [패턴 매칭 / 부분 검색](#%ED%8C%A8%ED%84%B4-%EB%A7%A4%EC%B9%AD--%EB%B6%80%EB%B6%84-%EA%B2%80%EC%83%89)
	- [[3장 - 정렬과 연산]](#3%EC%9E%A5---%EC%A0%95%EB%A0%AC%EA%B3%BC-%EC%97%B0%EC%82%B0)
		- [정렬](#%EC%A0%95%EB%A0%AC)
			- [사전식 순서에서 주의할 점](#%EC%82%AC%EC%A0%84%EC%8B%9D-%EC%88%9C%EC%84%9C%EC%97%90%EC%84%9C-%EC%A3%BC%EC%9D%98%ED%95%A0-%EC%A0%90)
			- [복수의 열을 지정해서 정렬하기](#%EB%B3%B5%EC%88%98%EC%9D%98-%EC%97%B4%EC%9D%84-%EC%A7%80%EC%A0%95%ED%95%B4%EC%84%9C-%EC%A0%95%EB%A0%AC%ED%95%98%EA%B8%B0)
		- [결과 행 제한하기](#%EA%B2%B0%EA%B3%BC-%ED%96%89-%EC%A0%9C%ED%95%9C%ED%95%98%EA%B8%B0)
			- [오프셋 지정](#%EC%98%A4%ED%94%84%EC%85%8B-%EC%A7%80%EC%A0%95)
		- [수치 연산](#%EC%88%98%EC%B9%98-%EC%97%B0%EC%82%B0)
			- [사칙연산](#%EC%82%AC%EC%B9%99%EC%97%B0%EC%82%B0)
			- [반올림함수: ROUND](#%EB%B0%98%EC%98%AC%EB%A6%BC%ED%95%A8%EC%88%98-round)
		- [문자열 연산](#%EB%AC%B8%EC%9E%90%EC%97%B4-%EC%97%B0%EC%82%B0)
			- [문자열 결합](#%EB%AC%B8%EC%9E%90%EC%97%B4-%EA%B2%B0%ED%95%A9)
			- [SUBSTRING](#substring)
			- [TRIM](#trim)
			- [CHARACTER_LENGTH](#character_length)
		- [날짜 연산](#%EB%82%A0%EC%A7%9C-%EC%97%B0%EC%82%B0)

<!-- /TOC -->
<!-- /TOC -->


## [1장 - 데이터베이스와 SQL]
### 데이터베이스
특정 데이터를 확인하고 싶을 때 간단하게 찾아낼 수 있도록 정리된 형태로, 하드디스크나 플래시메모리(SSD) 등 비휘발성 저장장치에 저장된다.

#### 데이터베이스 관리 시스템(DBMS; Database Management System)
데이터베이스를 효율적으로 관리하는 시스템

**> 왜 필요한가?**
1. **생산성**<br>
가장 기본적인 데이터 처리나 CRUD를 DBMS가 제공한다.<br>
   <img src=" ../assets/book-sql-first-step/1-6.jpeg" style="width: 400px; height: 200px;">

1. **기능성**<br>
복수의 사용자의 요청에 대응하거나, 대용량의 데이터를 저장하고 고속으로 검색하는 기능을 제공한다. 또한 데이터베이스 관리 기능을 유저가 확장할 수도 있어 유연한 개발이 가능하다. 이와 같은 데이터베이스를 다루는 여러 가지 기능을 DBMS가 제공한다.<br>
   <img src=" ../assets/book-sql-first-step/1-7.jpeg" style="width: 400px; height: 200px;">

1. **신뢰성**<br>
대규모 데이터베이스의 경우, 여러 대의 하드웨어를 구성하여 신뢰성을 높이고 성능을 향상시킬 수 있다.
   
   > 일부 DBMS는 컴퓨터를 여러 대 두고, 소프트웨어를 통해 확장성(Scalability)과 부하 분산(Load Balancing)을 구현한다. 이를 보통 ‘클러스터 구성’ 또는 ‘스케일 아웃’이라 부른다.
   
   또한 `export`, `import` 를 통한 간단한 백업이 가능핟.

#### SQL
- RDBMS의 데이터를 관리하기 위한 언어
- IBM이 개발한 SEQUEL(관계형 데이터베이스 조작용 언어)를 기반으로 만들어짐
- 종류
  DML(데이터 조작) + DDL(데이터 정의) + DCL(데이터베이스 제어)

---

### 다양한 데이터베이스
#### 관계형 데이터베이스 (RDB; Relational Database)

#### 데이터베이스 종류 
오래된 순서로. 
1. 계층형 데이터베이스
   - 계층 구조로 데이터를 저장하는 방식
   - EX) 하드디스크, DVD 파일시스템
2. 관계형 데이터베이스
   - 행과 열을 가지는 표 형식 데이터를 저장하는 형태의 데이터베이스
3. 객체지향 데이터베이스
   - 가능하면 객체 그대로를 데이터베이스의 데이터로 저장하는 방식
4. XML 데이터베이스
   - XML 형식으로 기록된 데이터를 저장하는 데이터베이스
5. Key-Value Store(KVS)
   - 키(key)와 그에 대응하는 값(value) 형태의 데이터를 저장하는 데이터베이스
   - From NoSQL(Not only SQL)
   - also called by 열 지향 데이터베이스

#### RDBMS 종류
1. Oracle
   - 오라클에서 개발한 RDBMS
   - RDBMS의 표준이라고 해도 무방
2. DB2
   - IBM
   - 한동안 IBM 컴퓨터에서만 구동됨 ➡️ 시장 점유율 확대 어려움의 이유
3. SQL Server
   - 마이크로소프트
   - 윈도우 플랫폼에서만 동작
4. PostgreSQL
   - 오픈소스 커뮤니티
5. MySQL
   - 오픈소스 커뮤니티
6. SQLite
   - 오픈소스 커뮤니티
   - 임베디드 시스템에 자주 사용됨. 
   - 매우 작음. 경량화

---

### 데이터베이스 서버
#### RDBMS의 클라이언트/서버
RDBMS는사용자 별로 데이터베이스 접근을 제한할 수 있기 때문에, 사용자 인증이 필요하다. 일반적으로 사용자 ID와 비밀번호로 인증한다.

<img src=" ../assets/book-sql-first-step/1-23.jpeg" style="width: 400px; height: 200px;">

#### 루프 백 접속
클라이언트에서 서버에 접속 시, 네트워크를 경유해서 PC의 서버로 되돌아오는 형태이다. PC 한 대로 클라이언트와 서버 모두 실행할 수 있지만 네트워크 기능이 필요하다.
  
<img src=" ../assets/book-sql-first-step/1-29.jpeg" style="width: 400px; height: 200px;">

---

## [2장 - 테이블에서 데이터 검색]

<img src=" ../assets/book-sql-first-step/2-10.jpeg" style="width: 400px; height: 200px;">

### 검색
  ```SQL
  SELECT * FROM 테이블명;
  ```

> 예약어와 데이터베이스 객체명은 대소문자를 구별하지 않는다.

---

#### 테이블 구조 참조하기
  - MySQL
    ```
    DESC 테이블명;
    ```
  - postgreSQL
    ```
    \d 테이블명;
    ```

---

#### 자료형
1. INTEGER 형<br>
   : 정수값을 저장할 수 있는 자료형.
2. CHAR 형<br>
   : 문자열을 저장할 수 있는 자료형.<br>
   언제나 고정된 길이로 데이터가 저장. 즉, 고정 길이 문자열
   따라서 최대 길이보다 작은 문자열 저장 시, 공백 문자로 나머지를 채운 후 저장하게 됨.
3. VARCHAR 형<br>
   : 문자열을 저장할 수 있는 자료형.<br>
   데이터 크기에 맞춰 저장공간의 크기 변경됨. 가변 길이 문자열
4. DATE 형<br>
   : 날짜값을 저장할 수 있는 자료형
5. TIME 형<br>
   : 시간을 저장할 수 있는 자료형

<img src=" ../assets/book-sql-first-step/2-16.jpeg" style="width: 300px; height: 180px;">

---

### 검색 조건 지정하기
  ```SQL
  SELECT * FROM 테이블명 WHERE 조건식;
  ```
  - 열 선택은 SELECT 구
  - 행 선택은 WHERE 구

---

### 패턴 매칭에 의한 검색
#### 패턴 매칭 / 부분 검색
특정 문자나 문자열이 포함되어 있는지를 검색하는 것으로, `LIKE` 술어를 사용하여 문자열의 일부분을 비교할 수 있다.

```sql
열명 LIKE 패턴
```

- 수치형 상수를 지정할 수 없다.
- 와일드카드(메타문자)를 사용할 수 있다.
  - `%` : 임의의 문자열. 빈 문자열도 포함.
  - `_` : 임의의 문자 하나
- 이스케이프 <br>
메타 문자에 `%`나 `_`를 포함하고 싶을 때 `\%` 또는 `\_`로 검색
- 문자열 상수 `'`는 2개를 연속으로 기술한다. 즉, `''`

---
## [3장 - 정렬과 연산]
### 정렬
```sql
SELECT 열명 FROM 테이블명 WHERE 조건식 ORDER BY 열명 [ASC(default) | DESC]
```
`ORDER BY` 구를 통해 검색 결과의 행 순서를 바꿀 수 있다.<br>
기본 정렬방법은 오름차순(`ASC`)이며, 열명 뒤에 `DESC` 기입 시 내림차순 정렬된다. <br>
`ORDER BY` 구를 따로 지정하지 않으면 일반적으로 데이터베이스 내부에 저장된 순서대로 출력되지만, 이마저도 일정하지 않다. 즉, `ORDER BY` 구 생략 시, 데이터 출력 순서는 데이터베이스 서버의 당시 상황에 따라 결정된다.
<br>

`ORDER BY`는 테이블에 영향을 주지 않는다. 즉, 서버에 클라이언트로 행 순서를 바꾸어 결과를 반환하는 것뿐, 저장장치에 저장된 데이터의 행 순서를 변경하는 것은 아니다.

#### 사전식 순서에서 주의할 점
수치형과 문자열형은 대소관계 계산 방법이 서로 다르다.<br>
수치형은 수치의 대소관계로, 문자열형은 사전식 순서로 비교한다.

**예시**
```text
mysql> DESC order_sample;
+----------+-------------+------+-----+---------+-------+
| Field    | Type        | Null | Key | Default | Extra |
+----------+-------------+------+-----+---------+-------+
| int_col  | int         | YES  |     | NULL    |       |
| char_col | varchar(10) | YES  |     | NULL    |       |
+----------+-------------+------+-----+---------+-------+

mysql> SELECT * FROM order_sample;
+---------+----------+
| int_col | char_col |
+---------+----------+
|       1 | 1        |
|      10 | 10       |
|      11 | 11       |
|       2 | 2        |
+---------+----------+
```

`int_col`은 수치형(INTEGER), `char_col`은 문자열형(VARCHAR)인 테이블이 있다.

`int_col` 열을 기준으로 정렬하면, 
```
mysql> SELECT * FROM order_sample ORDER BY int_col;
+---------+----------+
| int_col | char_col |
+---------+----------+
|       1 | 1        |
|       2 | 2        |
|      10 | 10       |
|      11 | 11       |
+---------+----------+
```

`char_col` 열을 기준으로 정렬하면,
```
mysql> SELECT * FROM order_sample ORDER BY char_col;
+---------+----------+
| int_col | char_col |
+---------+----------+
|       1 | 1        |
|      10 | 10       |
|      11 | 11       |
|       2 | 2        |
+---------+----------+
```

#### 복수의 열을 지정해서 정렬하기
```sql
SELECT 열명 FROM 테이블명 WHERE 조건식
ORDER BY 열명1 [ASC|DESC], 열명2 [ASC|DESC] ...
```



`NULL`은 데이터베이스 제품에 따라서 기준이 다르다. 특성상 대소비교를 할 수 없기에, ‘특정 값보다 큰 값‘ 또는 ’특정 값보다 작은 값‘ 두 가지로 나뉜다. 즉, `ORDER BY`로 지정한 열에서 `NULL` 값을 가지는 행은 가장 먼저 표시되거나 가장 나중에 표시된다. 예를 들어, MySQL의 경우는 `NULL` 값을 가장 작은 값으로 취급하고, PostgreSQL은 가장 큰 값으로 취급한다.

---

### 결과 행 제한하기
```sql
SELECT 열명 FROM 테이블명 LIMIT 행수 [OFFSET 시작행]
```

`LIMIT` 구를 사용해 표시할 행 수를 제한할 수 있다. `SELECT` 명령의 마지막에 지정해서 사용한다.

`WHERE` 을 사용해서 `LIMIT`과 동일한 결과를 얻을 수 있다. 다만 두 절은 기능과 내부처리 순서가 전혀 다르다.

이때 주의해야할 점은 `LIMIT` 구는 표준 SQL이 아니다. MySQL과 PostgreSQL에서만 사용 가능하다. 
SQL Server은 `TOP`을, Oracle은 `ROWNUM`이라는 열을 사용해 행을 제한할 수 있다.
- LIMIT
  ```sql
  SELECT * FROM sample LIMIT 3;
  ```
- TOP
  ```sql
  SELECT TOP 3 * FROM sample;
  ```
- ROWNUM
  ```sql
  SELECT * FROM sample WHERE ROWNUM <= 3;
  ```

<br>

#### 오프셋 지정

웹 시스템에서는 클라이언트의 브라우저를 통해 페이지 단위로 화면에 표시할 내용을 처리한다. 대량의 데이터를 하나의 페이지에 표시하는 것은 기능적으로도 속도 측면에서도 효율적이지 못하므로 일반적으로 페이지 나누기(pagination) 기능을 사용한다.

<img src=" ../assets/book-sql-first-step/3-pagination.jpeg" style="width: 200px; height:400px;">

몇번째 행부터 `LIMIT` 행 수만큼 출력할 것인지를 나타낼 때, `OFFSET`으로 몇번째 행부터를 나타낸다. `OFFSET`은 생략 가능하며 기본값은 0이다.

---

### 수치 연산
#### 사칙연산
| 연산자 | 연산 |
| --- | --- |
| + | 덧셈 |
| - | 뺄셈 |
| * | 곱셉 |
| / | 나눗셈 |
| % | 나머지 |

`SELECT` 구나, `WHERE` 구, `ORDER BY` 구에서 연산할 수 있다. 이때 `SELECT` 구에서 지정한 별칭을 `WHERE` 구에서는 사용할 수 없지만, `ORDER BY` 구에서는 사용할 수 있다. 그 이유는 보통 데이터베이스 서버에서 `WHERE` 구 ➡️ `SELECT` 구 ➡️ `ORDER BY` 구 순서로 내부처리를 진행하기 때문이다.

열의 별명은 예약어 `AS`를 사용해 지정한다. 키워드 `AS`는 생략해도 괜찮다.
```sql
SELECT *, price*quantity AS amount FROM sample;
```


명령구문을 분석할 때
더블쿼트(“”)로 둘러싸면 데이터베이스 객체의 이름으로 간주하고, 
싱글쿼트(‘’)로 둘러싸는 것은 문자열 상수이다.
더블쿼트로 둘러싸서 지정할 경우, 예약어와 같은 이름을 지정할 수 있다.

또한 연산에 `NULL`이 포함되면 결과는 항상 `NULL`이다.


#### 반올림함수: ROUND()
`ROUND(값, 반올림 자릿수)`<br>
반올림 자릿수를 입력하지 않으면 기본적으로 소수점 첫째 자리를 기준으로 반올림한다. 즉, 0으로 간주된다.

```SQL
mysql> SELECT  ROUND(123.456, -1),  ROUND(123.456, 0),  ROUND(123.456, 1);
+--------------------+-------------------+-------------------+
| ROUND(123.456, -1) | ROUND(123.456, 0) | ROUND(123.456, 1) |
+--------------------+-------------------+-------------------+
|                120 |               123 |             123.5 |
+--------------------+-------------------+-------------------+
```


반올림 외에 버림을 하고 싶은 경우에는 `TRUNCATE()` 함수로 계산할 수 있다.

> **DECIMAL 형**<br>
> 정수부와 소수부의 자릿수를 지정할 수 있는 자료형. 
> 즉, 소수점을 포함하는 수치를 저장하는 자료형

그 외에도 삼각함수 `SIN`, `COS`, 루트를 계산하는 `SQRT`, 대수를 계산하는 `LOG` 등수많은 함수가 있다.

---

### 문자열 연산
#### 문자열 결합
2개의 열 데이터를 모아서 1개의 열로 처리할 때 사용한다.

| 연산자 / 함수 | 데이터베이스 |
| --- | --- |
| + | SQL Server |
| \|\| | Oracle, DB2, PostgreSQL |
| CONCAT | MySQL |

#### SUBSTRING()
`SUBSTRING(값, 시작 위치, 추출 문자 개수)`<br>
문자열의 일부분을 계산해서 반환해주는 함수로, 데이터베이스에 따라서 함수명 SUBSTR인 경우도 있다.

#### TRIM()
`TRIM(값, 제거 문자)`<br>
문자열의 앞뒤로 여분의 스페이스가 있는 경우 이를 제거해주는 함수로, 문자열 중간에 존재하는 스페이스는 제거되지 않는다.  제거 문자 지정 시, 스페이스 이외의 문자를 제거할 수 있다.

CHAR 형(고정길이 문자열형)은 문자열의 길이가 고정되고 남은 공간은 스페이스로 채워지기 때문에, 빈 공간을 채우기 위해 사용한 스페이스를 제거하기 위해 많이 사용된다.

#### CHARACTER_LENGTH()<br>
문자열의 길이를 반환하는 함수. CHAR_LENGTH로 줄여서 사용해도 된다.
`OCTET_LENGTH()` 는 문자열의 길이를 바이트 단위로 반환하는 함수이다.

> **문자세트(character set)**<br>
> 문자와 그 문자의 인코딩의 집합<br>
> character set에 따라서 한 문자가 몇 바이트인지는 다를 수 있다. 따라서 문자열 데이터의 길이는 character set에 따라 다르다.
> 
> <img src=" ../assets/book-sql-first-step/3-4-table.jpeg" style="width: 400px; height: 120px;">

---

### 날짜 연산

사칙 연산이 가능하다.<br>
날짜시간 데이터를 연산하면 결괏값으로 날짜시간 유형의 데이터를 반환하는 경우도 있으며, 기간(간격)의, 차를 나타내는 기간형(interval) 데이터를 반환하는 경우도 있다. 

**시스템 날짜**<br>
하드웨어 상의 시계로부터 실시간으로 얻을 수 있는 일시적인 데이터

표준 SQL에서는 `CURRENT_TIMESTAMP`, `CURRENT_DATE`, `CURRENT_TIME`을 지원한다.

```sql
mysql> SELECT CURRENT_TIMESTAMP;
+---------------------+
| CURRENT_TIMESTAMP   |
+---------------------+
| 2025-02-21 17:09:17 |
+---------------------+
```

```sql
mysql> SELECT CURRENT_DATE;
+--------------+
| CURRENT_DATE |
+--------------+
| 2025-02-21   |
+--------------+
```

```sql
mysql> SELECT CURRENT_TIME;
+--------------+
| CURRENT_TIME |
+--------------+
| 17:09:44     |
+--------------+
```

<br>

Oracle의 경우 `TO_DATE(문자열 데이터, 날짜 서식)` 함수를 사용하여 문자열 데이터를 날짜 서식에 맞게 날짜형 데이터로 변환할 수 있다. 반대로 `TO_CHAR()` 함수를 사용하여 날짜형 데이터를 서식에 맞게 변환해 문자열 데이터로 출력할 수 있다.
[MySQL](https://dev.mysql.com/doc/refman/8.4/en/date-and-time-functions.html#function_date-format)의 경우, `DATE_FORMAT()` 함수를, [PostgreSQL](https://www.postgresql.org/docs/current/functions-formatting.html)은 `TO_DATE`, `TO_TIMESTAMP` 함수를 제공한다.

```sql
mysql> SELECT DATE_FORMAT(CURRENT_TIMESTAMP, '%Y %M %D');
+--------------------------------------------+
| DATE_FORMAT(CURRENT_TIMESTAMP, '%Y %M %D') |
+--------------------------------------------+
| 2025 February 21st                         |
+--------------------------------------------+
```
<br>


날짜시간형 데이터는 기간형 수치데이터(INTERVAL)와 덧셈 및 뺄셈을 할 수 있다. 연산 결과로 날짜시간형 데이터가 반환된다.

- MySQL
   ```SQL
   mysql> SELECT DATE('2025-02-17') + INTERVAL 1 DAY;
   +-------------------------------------+
   | DATE('2025-02-17') + INTERVAL 1 DAY |
   +-------------------------------------+
   | 2025-02-18                          |
   +-------------------------------------+
   ```

- PostgreSQL
   ```SQL
   postgres=# SELECT CURRENT_DATE + INTERVAL '1 DAY' AS result;
         result        
   ---------------------
   2025-02-23 00:00:00
   ```

`DATEDIFF()` 함수로 날짜형 간의 뺼셈도 할 수 있다.

<br>

> **날짜 및 시간 함수 공식 문서**<br>
> [MySQL](https://dev.mysql.com/doc/refman/8.4/en/date-and-time-functions.html#function_date)
> [PostgreSQL](https://www.postgresql.org/docs/current/functions-datetime.html)
>
