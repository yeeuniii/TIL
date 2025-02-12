# REST API
## API
> Application Programming Interface
>
애플리케이션 간에 데이터를 주고받을 수 있도록 정해진 규칙이나 프로토콜

## REST
> REpresentational State Transfer
>

### REST 구성 요소
> 자원을 이름으로 식별하여, 해당 자원의 상태(행위)를 주고 받는 것
>
- **자원(Resource)** by URI<br>
URI를 통해 사용할 자원을 명시한다. 
- **행위(Verb)** by HTTP Method

- **표현(Representation of Resource)**<br>
다양한 형태(JSON, XML, TEXT, RSS etd)로 리소스를 표현할 수 있다. <br>
보통 JSON 또는 XML로 데이터를 주고 받는 것이 일반적이다.

## REST API
HTTP 통신을 REST 설계 원칙을 준수해서 개발한 API<br>
RESTful API 라고도 부른다.<br>


### REST 설계 원칙
1. **Uniform Interface(균일한 인터페이스)**<br>
- 리소스 식별(Identification of Resources)<br>
URI로 리소스가 식별되어야 한다. 
- 표현을 통한 자원 조작 (Manipulation Of Resource through Representations)<br>
클라이언트가 표현을 통해 리소스를 조작할 수 있어야 한다. <br>
즉, 특정 리소스를 나타내는 URL의 변경 없이, 표현을 활용해서 리소스를 조작할 수 있다. 예로, HTTP 헤더의 `Accept` 헤더를 활용할 수 있다.
- 자기 서술적인 메시지(Self-descriptive messages)<br>
요청(request), 응답(response)와 같은 메시지는, 메시지 그 자체만 보고 무슨 의미인지 파악할 수 있을 정도로 정보가 담겨있어야 한다.
- HATEOAS (hypermedia as the engine of application state)<br>
애플리케이션의 상태가 Hyperlink를 이용해 전이되어야 한다.

2. **클라이언트 - 서버 분리**<br>
클라이언트와 서버는 서로 완전히 독립되어야 한다. <br>
클라이언트는 데이터를 서버에 요청하고, 서버는 클라이언트의 요청에 대한 데이터를 응답한다. <br>
또한 '사용자 인터페이스에 대한 관심사'와 '데이터 스토리지 문제에 대한 관심사'를 분리 ➡️ 확장성. 명확한 역할 분리. 

3. **Stateless(무상태성)**<br>
모든 요청에는 처리에 필요한 모든 정보가 포함되어야 한다. 

4. **Cacheable(캐시 가능성)**<br>
가능한 경우 클라이언트나 서버에서 리소스를 캐시할 수 있어야 한다. 
이를 통해 데이터 전달의 속도를 향상시킬 수 있다. 

5. **Layered System(계층화된 시스템 아키텍처)**<br>
클라이언트나 서버가 최종 애플리케이션과 통신하는지 중개자와 통신하는지 알 수 없도록 설계해야 한다. 예로, API 서버, DB 서버, 인증 서버로 서버 확장성을 보장할 수 있다. 

6. Code on Demend(코드 온디맨드), 선택사항 <br>
일반적으로 정적 리소스를 전송하지만, 경우에 따라서 응답에 실행 코드가 포함될 수도 있다. 즉, 서버가 클라이언트에서 실행시킬 수 있는 로직을 전송하여 클라이언트의 기능을 확장시킬 수 있다. 

### 목적
- 사용하기 쉽고, 이해하기 쉽다.
- 일관적인 컨벤션을 통해 API의 이해도 및 호환성을 높인다.
- 성능 향상의 목적을 갖지 않는다.
- URI와 HTTP Method만으로도 통신의 의도를 파악할 수 있다.

### 참고
[REST, RESTful API 란 무엇인가요?](https://www.ibm.com/kr-ko/topics/rest-apis)<br>
[REST의 기본 원칙 6가지](https://jaeseongdev.github.io/development/2021/06/15/REST%EC%9D%98-%EA%B8%B0%EB%B3%B8-%EC%9B%90%EC%B9%99-6%EA%B0%80%EC%A7%80/)<br>
[[네트워크] REST API / Restful API란? (feat. 원칙과 네이밍 규칙)](https://ziszini.tistory.com/91)