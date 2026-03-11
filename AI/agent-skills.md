# AI Agent Skills

> A simple, open format for giving agents new capabilities and expertise.  
> : 에이전트에게 새로운 기능과 전문 지식을 제공하기 위한 간단하고 열려있는 기능

## 1. Agent Skills란?

* **핵심 개념**: AI 모델(뇌)에 **'손과 발'** 을 쥐여주어 실제로 행동할 수 있게 만드는 것.
* **효과**: 반복 작업의 표준화, 팀 간 지식 공유, 복잡한 작업의 캡슐화를 가능하게 한다.

## 2. 왜 Skills가 필요한가? (Context File의 한계)

* 기존에는 `AGENTS.md`나 `GEMINI.md` 같은 컨텍스트(시스템 프롬프트) 파일로 프로젝트 전반의 정보를 제공했다.
* **문제점**:
  * 특정 작업에 대한 세부 지침까지 모두 담기에는 한계가 있음.
  * 컨텍스트 파일이 너무 길어지면 AI가 읽는 데 시간이 오래 걸리고, 토큰 사용량이 증가하며, 중요한 정보가 누락될 수 있음.
* **해결책**: 이를 해결하기 위해 **Skills**가 등장함.

## 3. Skills의 구조와 특징

AI 에이전트가 특정 작업을 수행할 때 참조하는 지침, 스크립트, 리소스의 집합이다.

### 폴더 구조 예시

```text

my-skill/
├── SKILL.md          # Required: instructions + metadata
├── scripts/          # Optional: executable code
├── references/       # Optional: documentation
└── assets/           # Optional: templates, resources

```
- 최상위 경로의 .agents/ (또는 .gemini/) 폴더 안의 skills 폴더 안에 위치
- 디렉터리의 집합

### 주요 특징

* **비유**: 컨텍스트 파일이 **'우리 팀의 개발 문화'** 라면, Skills는 **'특정 업무의 작업 매뉴얼'** 이다.
* **Self-documenting**: 자기 문서화가 가능함.
* **Extensible**: 확장이 용이함.
* **Portable**: 휴대성이 좋아 어디서든 사용 가능함.

## 4. 에이전트의 Skill 실행 프로세스 (Progressive Disclosure)

에이전트는 **단계적 노출(Progressive Disclosure)** 원칙에 따라 필요한 정보만 읽어 효율을 높인다.

1. **Discovery (검색)**: 시작할 때 에이전트는 각 스킬의 이름과 설명만 로드한다.
2. **Activation (활성화)**: 작업이 결정되면 에이전트는 `SKILL.md` 내부의 지침(Instructions)과 컨텍스트를 읽는다.
3. **Execution (실행)**: 에이전트는 지침에 따라 선택적으로 `references`를 로드하거나 `scripts`를 실행한다.

## 5. 권장 사항 및 철학

* **토큰 관리**: `SKILL.md` 파일은 **5,000 토큰 이하**로 유지하는 것이 권장된다.
* **AI 활용 능력의 변화**:
  * AI 활용 능력 = 모델 성능 < **어떤 Skills를 가지고 있느냐**
  * AI를 잘 쓰는 사람 = 프롬프트를 잘 치는 사람 < **어떤 Skills를 설계하고 조합하느냐**


### 추가 팁
skills 집합 사이트 https://skills.sh/

### 참고
https://agentskills.io/what-are-skills  
https://daleseo.com/agent-skills/  

