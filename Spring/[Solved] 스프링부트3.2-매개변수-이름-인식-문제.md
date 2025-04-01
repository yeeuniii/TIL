## 스프링부트 3.2 매개변수 이름 인식 문제

### 사건의 발단
여느때와 같이 스프링 개발 자알 하던 중에 온 연락

<img src="../assets/spring-solved-argument-type.png" style="width: 500px; height: 200px;">

갑자기 이게 무슨..? 

```text
[ERROR] o.a.c.c.C.[.[.[.[dispatcherServlet]: Servlet.service() for servlet [dispatcherServlet] in context with path [] threw exception [Request processing failed: java.lang.IllegalArgumentException: Name for argument of type [java.lang.String] not specified, and parameter name information not available via reflection. Ensure that the compiler uses the '-parameters' flag.] with root cause
java.lang.IllegalArgumentException: Name for argument of type [java.lang.String] not specified, and parameter name information not available via reflection. Ensure that the compiler uses the '-parameters' flag.
```

같은 코드로 IntelliJ에서는 문제가 되지 않는데, VSCode에서만 위와 같은 문제가 발생한다. 뭐가 문제인걸까?

### 원인 파악
좀 찾아보니 스프링부트 3.2 버전부터는 자바 컴파일러에 -parameters 옵션을 넣어주어야만 어노테이션에 이름(name)을 생략할 수 있다고 한다.

주로 다음 어노테이션에서 위와 같은 에러가 발생한다고 한다.
`@RequestParam`, `@PathVariable`, `@Autowired`, `@ConfigurationProperties`

해결 방안으로는
1. 어노테이션에 이름 생략하지 않고, 지정해주기
	- 모든 코드를 확인하고 추가해주어야 하기때문에 시간 비용 부담이 크다.
2. 컴파일 시점에 -parameters 옵션 적용
3. Gradle을 사용해서 빌드하고 실행 (권장)

IntelliJ에서는 Gradle로 프로젝트를 빌드하고 실행시키도록 설정되어있어서 3번 방법을 사용하고 있던 것이다.

따라서 VSCode에서는 `Gradle for Java` 확장자를 설치하여 bootRun 명령어로 프로젝트를 실행시키게 하면 3번 방법으로 문제를 해결할 수 있다. 

근데 아직도 의문인 거는 이때까지 VSCode로 잘 실행해왔던 거로 알고 있는데 
왜 갑자기 이러한 문제가 발생한 것인지는... 아직도 미지수다 ㅎ

### 참고
[인프런 김영한님의 자주 하는 질문](https://docs.google.com/document/d/1j0jcJ9EoXMGzwAA2H0b9TOvRtpwlxI5Dtn3sRtuXQas/edit?tab=t.0#heading=h.b1yk4ued1pxo)
[공식 문서](https://github.com/spring-projects/spring-framework/wiki/Spring-Framework-6.1-Release-Notes)