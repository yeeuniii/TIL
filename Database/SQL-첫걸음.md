# SQL 첫걸음
## [1장 - 데이터베이스와 SQL]
### 1강. 데이터베이스
- 특정 데이터를 확인하고 싶을 때 간단하게 찾아낼 수 있도록 정리된 형태
- 하드디스크나 플래시메모리(SSD) 등 비휘발성 저장장치에 저장됨

#### 데이터베이스 관리 시스템(DBMS; Database Management System)
데이터베이스를 효율적으로 관리하는 시스템

> 왜 필요한가?
1. 생산성<br>
가장 기본적인 데이터 처리, CRUD를 DBMS가 제공함<br>
<img src="https://github.com/user-attachments/assets/99a70ab8-516d-4f15-bf1d-e7cb6c7d16de" style="width: 400px; height: 200px;">
1. 기능성<br>

   <img src="https://github.com/user-attachments/assets/205e4748-37f3-43f7-bccc-31da3edc219f" style="width: 400px; height: 200px;">
1. 신뢰성<br>
대규모 데이터베이스의 경우, 여러 대의 하드웨어를 구성하여 신뢰성을 높임 + 성능 향상
   > 일부 DBMS는 컴퓨터를 여러 대 두고, 소프트웨어를 통해 확장성(Scalability)과 부하 분산(Load Balancing)을 구현함.<br> 이를 보통 ‘클러스터 구성’ 또는 ‘스케일 아웃’이라 부름.
   
   export, import 를 통한 간단한 백업 가능

#### SQL
- RDBMS의 데이터를 관리하기 위한 언어
- IBM이 개발한 SEQUEL(관계형 데이터베이스 조작용 언어)를 기반으로 만들어짐
- 종류
  DML(데이터 조작) + DDL(데이터 정의) + DCL(데이터베이스 제어)

---

### 2강. 다양한 데이터베이스
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

### 3강. 데이터베이스 서버
#### RDBMS의 클라이언트/서버
- RDBMS는사용자 별로 데이터베이스 접근을 제한할 수 있기 때문에, 사용자 인증이 필요함
  - 사용자 ID와 비밀번호로 실행<br>

   <img src="https://github.com/user-attachments/assets/b9700caf-50e3-44c7-9d82-2b4b04ab3837" style="width: 400px; height: 200px;">

<br>

- 루프 백 접속
  - 클라이언트에섯 서버에 접속 시, 네트워크를 경유해서 PC의 서버로 되돌아오는 형태
  - PC 한 대로 클라이언트와 서버 모두 실행할 수 있지만 네트워크 기능 필요. 
  
  <img src="https://github.com/user-attachments/assets/d7998387-d418-4dc9-a6e4-6452e9eb90df" style="width: 400px; height: 200px;">

---

## [2장 - 테이블에서 데이터 검색]
#### 검색
  ```SQL
  SELECT * FROM 테이블명;
  ```

> 예약어와 데이터베이스 객체명은 대소문자를 구별하지 않는다.

<img src="https://github.com/user-attachments/assets/2093ddfd-1706-4267-a46d-86a550c56b3f" style="width: 400px; height: 200px;">

<br>

#### 테이블 구조 참조하기
  - MySQL
    ```
    DESC 테이블명;
    ```
  - postgreSQL
    ```
    \d 테이블명;
    ```

#### 자료형
1. INTEGER 형<br>
   정수값을 저장할 수 있는 자료형.
2. CHAR 형<br>
   문자열을 저장할 수 있는 자료형.<br>
   언제나 고정된 길이로 데이터가 저장. 즉, 고정 길이 문자열
   따라서 최대 길이보다 작은 문자열 저장 시, 공백 문자로 나머지를 채운 후 저장하게 됨.
3. VARCHAR 형<br>
   문자열을 저장할 수 있는 자료형.<br>
   데이터 크기에 맞춰 저장공간의 크기 변경됨. 가변 길이 문자열
4. DATE 형<br>
   날짜값을 저장할 수 있는 자료형
5. TIME 형<br>
   시간을 저장할 수 있는 자료형
<img src="https://github.com/user-attachments/assets/46c5c2f0-a8ee-436c-8bf6-485a31af8c86" style="width: 300px; height: 180px;">

<br>

#### 검색 조건 지정하기
  ```SQL
  SELECT * FROM 테이블명 WHERE 조건식;
  ```
  - 열 선택은 SELECT 구
  - 행 선택은 WHERE 구