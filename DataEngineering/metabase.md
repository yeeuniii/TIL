# 메타베이스(metabase)
SQL 지식 없이도 데이터를 시각화하고 분석할 수 있는 오픈소스 Business Intelligence (BI) 도구

> https://www.metabase.com/

## 구조
### Product 
- **Query**: 연결된 데이터베이스에서 원하는 데이터를 추출할 수 있는 쿼리
- **Dashboard**: Query를 통해 추출한 데이터의 시각화와 분석을 위해 설계된 인터페이스
- **Model**: 테이블 생성 및 저장, 시각화 등 다양한 기능 제공
  - 새로운 테이블을 생성하고 저장하여 추후 해당 테이블을 메타베이스에서 사용할 수 있음
- **Collection**: product를 저장하는 폴더

### Admin
- **People**: 메타베이스 접근 권한이 있는 구성원
- **Group**: People 각각이 속한 그룹
- **Permission**: Database, Table, Native query editing, Collection에 대한 접근 권한
  - Group 단위로 설정 가능

### Permission (접근 권한)
#### Collection
대시보드나 저장된 쿼리 등 만들어진 결과물(product)의 접근 권한이다. 
 - **Curate**: 편집 및 View 권한
 - **View**: 조회 권한
 - **No access**: 접근 불가

<br>
아래는 사용자가 새로운 쿼리를 직접 생성하거나(self-service) raw data를 조회할 수 있는지에 대한 권한들이다. 

#### Database
- **Unrestricted**: 이 디비를 사용하는 Product에 접근 가능.
- **Granular**: 이 디비의 특정 테이블을 사용하는 Product에만 접근 가능.
- **No self-service**: 그룹이 해당 디비에 접근 불가능.

#### Table
Database 권한이 Granular로 설정된 경우, 하위 Access 권한.
- **Unrestricted**: 이 테이블을 사용하는 Product에 접근 가능.
- **No self-service**: 테이블에 접근 불가능.

#### Native query editing
- **Yes**: 그룹의 쿼리 작성 및 수정 허용.
- **No**: 그룹의 쿼리 작성 및 수정 제한.

<br>

예를 들어, 특정 그룹이 어떤 database 권한이 no self-service인 경우 이 데이터베이스로 만든 product가 있는 collection에 view 권한이 있다면 대시보드에 조회 가능하다. 하지만 이 대시보드의 원본 쿼리를 조회하거나 수정할 수 없다. 



### 참고
[잡플래닛 메타베이스 도입기](https://techspace.jobplanet.co.kr/%EC%9E%A1%ED%94%8C%EB%9E%98%EB%8B%9B-%EB%A9%94%ED%83%80%EB%B2%A0%EC%9D%B4%EC%8A%A4-%EB%8F%84%EC%9E%85%EA%B8%B0-21377#1_%EB%93%A4%EC%96%B4%EA%B0%80%EB%A9%B0_0)   

