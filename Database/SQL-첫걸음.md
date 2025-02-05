# SQL 첫걸음
## [1장 - 데이터베이스와 SQL]
### 1강. 데이터베이스
- 특정 데이터를 확인하고 싶을 때 간단하게 찾아낼 수 있도록 정리된 형태
- 하드디스크나 플래시메모리(SSD) 등 비휘발성 저장장치에 저장됨

#### 데이터베이스 관리 시스템(DBMS; Database Management System)
데이터베이스를 효율적으로 관리하는 시스템

> 왜 필요한가?
1. 생산성
   - 가장 기본적인 데이터 처리, CRUD를 DBMS가 제공함
![image](https://github.com/user-attachments/assets/99a70ab8-516d-4f15-bf1d-e7cb6c7d16de)
2. 기능성
![image](https://github.com/user-attachments/assets/205e4748-37f3-43f7-bccc-31da3edc219f)
3. 신뢰성
   - 대규모 데이터베이스의 경우, 여러 대의 하드웨어를 구성하여 신뢰성을 높임 + 성능 향상
   > 일부 DBMS는 컴퓨터를 여러 대 두고, 소프트웨어를 통해 확장성(Scalability)과 부하 분산(Load Balancing)을 구현함.<br> 이를 보통 ‘클러스터 구성’ 또는 ‘스케일 아웃’이라 부름.
   - export, import 를 통한 간단한 백업 가능

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

