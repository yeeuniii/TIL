# JPA 더티 체킹(Dirty Checking)

`@Transactional` 어노테이션을 사용한 메서드에서 데이터를 수정하고 따로 save() 메서드를 호출하지 않아도, 메서드가 정상 종료하면 데이터베이스에 데이터의 수정이 반영된다. 
JPA는 어떻게 이런 행위를 알아서 해주는 걸까?

이는 바로 `Dirty Checking` 때문이다.

## Dirty Checking 
여기서 `Dirty` 는 더러운. 이 아닌 "상태에 변화가 생겼다."의 의미이다.
즉, `Dirty Checking`은 **상태 변경 검사**이다.

JPA는 트랜잭션이 끝나는 시점에 상태에 변화가 생긴 모든 엔티티들을 데이터베이스에 자동으로 반영해준다. 이때 변화가 있다의 기준은 최초 조회 상태이다. 
JPA에서는 엔티티를 조회하면 해당 엔티티의 조회 상태를 `스냅샷`으로 만든다.
그리고 트랜잭션이 끝나는 시점과 비교해서, 상태에 변화가 생겼다면 update 쿼리를 데이터베이스에 전달한다.

이때, 더티 체킹의 대상은 영속성 컨텍스트가 관리하는 엔티티이다.

더티 체킹의 실행 시점은 트랜잭션이 끝날 때, 즉 commit 될 때이다.

### 참고
[더티 체킹 (Dirty Checking)이란?](https://jojoldu.tistory.com/415)