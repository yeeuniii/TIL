## OLAP vs OLTP (비즈니스 관점)

**OLAP(OnLine Analytical Processing)**: 온라인 분석 처리. 
**OLTP(Online Transaction Processing)**: 온라인 트랜잭션 처리. 

OLAP는 복잡한 데이터 분석에 사용되는 반면, OLTP는 대규모 온라인 트랜잭션의 실시간 처리에 사용됨.   
고유한 데이터 처리 및 저장 요구 사항에 따라 구분되어 온 별개의 프로세스.

|  | OLAP | OLTP |
| --- | --- | --- |
| 용도 | 대량의 데이터를 분석하여 의사 결정을 지원 (데이터 쓰기 < 데이터 읽기) | 실시간 트랜잭션을 관리 및 처리 (데이터 읽기 < 데이터 쓰기) |
| 저장소 크기 | 대용량 스토리지 필요. TB / PB 규모 | 스토리지 요구사항이 비교적 작음. GB 규모 |
| 쿼리 성능 / 속도 | 응답 시간이 상대적으로 더 길다. (초 ~ 분 단위) | 응답 시간이 짧음 (밀리초 단위) |
| 사용 사례 | 게임 유저 로그 데이터를 통한 레벨 데이터를 분석 | 결제 처리, 고객 데이터 관리 및 주문 처리 |

<br>

## Row-oriented vs Column-oriented (저장 구조 관점)
|  | **Column-oriented** | **Row-oriented** |
| --- | --- | --- |
| 데이터 저장 방식 | 열 단위로. 특정 열의 모든 값은 여러 행에 걸쳐 그룹화됨 | 행 단위로 |
| 쿼리 성능 | 읽기 작업이 많은 작업에서 성능이 우수하지만, 쓰기 작업에서는 속도가 느리다.  | 단일 레코드의 CRUD에 최적화되어 있다.<br>읽기 작업이 많은 경우, 전체 행을 스캔해야 되기 때문에 속도가 느릴 수 있다. 하지만 쓰기 작업의 경우, 업데이트가 단일 행으로 제한되므로 속도가 더 빠르다.  |
| 압축 | 유사한 데이터 유형끼리 저장되므로, 고급 악축 기능을 지원함.  | 한 행에 여러 데이터 유형이 혼재하면, 상대적으로 압축 효율이 떨어짐 |
| 유연성 | 스키마 변경에 대한 유연성 떨어짐 | 스키마 변경 유연 |
| 확장성 | 수평 확장 수행. <br>클러스터에 노드 추가. 서버 수를 늘려 부하 분산 및 응답 시간을 단축 | 수직 확장 수행. <br>데이터베이스 리소스(cpu, ram 등)를 늘림. 병렬 처리가 제한적 |
| 사용 사례 | Data warehousing, 빅데이터 분석 및 BI, IoT | 거래 시스템, OLTP |
| 대표 도구 | StarRocks, ClickHouse, Snowflake, Google BigQuery | 기존의 전통적인 RDBMS<br>MySQL, PostgreSQL, Oracle, MariaDB etc.  |


### Column 기반 디비는 왜 분석에 유리한가?

1. I/O 효율  
  분석에 필요한 컬럼만 조회해올 수 있음. 많은 양의 데이터에도 유리
2. 압축 
  데이터는 비슷한 유형끼리 모여 있을 때 압축이 잘 됨. 
3. 벡터화 연산  
   OLAP 엔진들의 CPU는 데이터를 한 개씩 처리하지 않고, 수천 개를 한 묶음(Vector)으로 만들어 CPU 캐시에서 한 번에 연산함. 



### 참고
[[AWS] OLAP과 OLTP의 차이점은 무엇인가요?](https://aws.amazon.com/ko/compare/the-difference-between-olap-and-oltp/)  
[Columnar Databases vs. Row-Oriented Databases: Which to Choose?](https://www.tigerdata.com/learn/columnar-databases-vs-row-oriented-databases-which-to-choose)