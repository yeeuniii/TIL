# Chatper3

#### 목표 - LangChain과 친해지고, 그 강력함을 느껴보는 것

<br>

```python
from langchain.llms.openai import OpenAI
```
- text-davinci-003 (Legacy) 사용
- auto-complete 제공되지만, chat에 최적화되어있지 않음

<br>

```python
from langchain.chat_models import ChatOpenAI
```
- gpt-3.5-turbo
- chat을 위해 최적화되어있는 모델
- text-davinci-003 보다 1/10의 비용
- 가능하면 앞으로 우리는 이 모델을 사용하자!!

<br>

- perdict 메서드
  - string 인풋 ➡ string 아웃풋
- .env 파일 안에 반드시 `OPEN_API_KEY` 라는 환경변수가 있어야 함

<br>

## 3.1 Predict Messages
```python
chat = ChatOpenAI()
```
- 생성자(constructure)을 통해 model의 설정을 변경할 수 있음
  - max_tokens : 반환하는 결과의 최대 토큰
  - temperature : 모델이 얼마나 창의적인 결정을 하는지. 값이 클수록 높은 창의성과 무작위성 가짐

<br>

```python
from lanchain.schema import HumanMessage, AIMessage, SystemMessage
```
- HumanMessage
- AIMessage : AI에 의해 보내짐
- SystemMessage : 우리가 LLM에 설정들을 제공하기 위한 message

- perdict_message
  - message 리스트 인풋

## 3.2 Prompt Templates

text를 perdict

```python
from langchain.prompts import PromptTemplate, ChatPromptTemplate
```
- ChatPromptTemplate : template을 message로부터 생성
- PromptTemplate : string으로 template 만들기

<br>

- {} 중괄호로 placehorder 을 만들어줄 수 있음
- ex)
```python
from langchain.prompts import PromptTemplate
from langchain.chat_models import ChatOpenAI

tempalte = PromptTemplate(
	"What is the distance between {country_a} and {country_b}"
)

template.format(country_a="Maxico", country_b="Thailand")

chat = ChatOpenAI(temperature=0.1)

chat.predict(prompt)
```

- template.format과 predict를 반복해서 작성해야한다는 불편함 있음
- 마지막 section 에서 위 과정을 한 줄의 코드로 할 수 있는 방법 배움

<br>

```python
from langchain.prompts import ChatPromptTemplate
from langchain.chat_models import ChatOpenAI

template = ChatPromptTemplate([
	("system", "You are a geography expert. And you only reply in {language}."),
	("ai", "Ciao, mi chhiamo {name}!"),
	("human", "What is the distance between {country_a} and {country_b}")
])

prompt = template.format_messages(
	language="Greek",
	name="Socrates",
	country_a="Maxico",
	country_b="Thailand"
)

chat = ChatOpenAI(temperature=0.1)

chat.perdict_message(prompt)
```
- #3.1의 과정을 개선된 코드로 구현 가능
- 변수(variable)들을 검증(validate)할 수 있음
- 커스텀 편리해짐

<br>

## 3.3 OutputParser and LCEL
- Output Parser : LLM의 output을 구조별로 파싱할 수 있게 해줌
  - 왜? LLM의 응답(Response)을 변형해야할 때가 있기 때문
- LLM은 항상 텍스트로만 응답함

<br>

- 예시) 응답(Response) ➡️ List
```python
from langchain.schema import BaseOutputParser

class CommaOutputparser(BaseOutputParser):
	def parse(self, text): # 반드시 구현해야 함
		items =  text.strip().split(",")
		return list(map(str.strip, items))

p = CommaOutputParser()

p.parse("Hello,how,are,you") # 반환값: ["Hello", "how", "are", "you"]
```
- base class 같은 것.
  - 얘를 확장해서 Output parser 구현할 것

<br>

```python
from langchain.prompts import ChatPromptTemplate
from langchain.chat_models import ChatOpenAI

template = ChatPromptTemplate([
	("system", "You are a list generating machine. Everything you are asked will be answered with a comma seperated list of max {max_items} in lower case. Do NOT reply with anything else."),
	("human", "{qeustion}")
])

prompt = template.format_messages(
	max_items=10,
	question="What are the planets?"
)

chat = ChatOpenAI(temperature=0.1)

result = chat.perdict_message(prompt)

p = CommaOutputParser()

p.parse(result.content)
```

- 위 과정은 너무 길다. 지금은 2025년!!
- 따라서 지금부터 `chain` 이라는 것을 만들어보자.
  - 아래 세 가지 (Template, Language Model, Output Parser) 만 남겨두고, 

```python
from langchain.prompts import ChatPromptTemplate
from langchain.chat_models import ChatOpenAI
from langchain.schema import BaseOutputParser

class CommaOutputparser(BaseOutputParser):
	def parse(self, text): # 반드시 구현해야 함
		items =  text.strip().split(",")
		return list(map(str.strip, items))

chat = ChatOpenAI(temperature=0.1)

template = ChatPromptTemplate([
	("system", "You are a list generating machine. Everything you are asked will be answered with a comma seperated list of max {max_items} in lower case. Do NOT reply with anything else."),
	("human", "{qeustion}")
])
``` 

- chain이란, 기본적으로 모든 요소를 합쳐주는 역할을 함
- `|` : LangChain의 아주 환상적인 operator


```python
chain = template | chat | CommaOutputParser()

chain.invoke({
	max_items=5,
	question="What are the pokemons?"
})
```
- 이 Chain은 Template, Language Model, Output Parser 로 구성
- LangChain이 내부적으로, (invoke 메서드)
  - .format_messages 메서드 호출
  - chat.predict 호출
  - parse 호출
- chain끼리의 결합도 가능


<br>

## 3.4 Chaining Chains

LangChain expression language(표현 언어)

- chain의 invoke 메서드 호출 시, prompt가 가장 먼저 호출 됨. w/ dictionary 타입의 입력값
- chain은 단지 componenet의 그룹 혹은 나열

<br>

| Component | Input Type | Output Type |
| --- | --- | --- |
| Prompt | Dictionary | PromptValue |
| LLM, ChatModel | Single string, list of chat messages or PromptValue | ChatMessage |
| OutputParser | The output of an LLM or ChatModel | Depends on the parser |
> [공식 문서](https://python.langchain.com/docs/concepts/runnables/#input-and-output-types)
>

---
<br>

- 두 개의 chain을 연결할 때, 첫번째 chain의 결과값을 두번째 chain의 입력값으로 넣고 싶다면, 
```python
final_chain = {"second_input": first_chain} | second_chain

final_chain.invoke({
	"first_input": "blabla"
})
```
- chain에 context object를 입력해준 것


```python
from langchain.chat_models import ChatOpenAI
from langchain.callbakcs import StreamingStdOutCallbackHandler

chat = ChatOpenAI(
	temperature=0.1,
	streaming=True,
	callbakcs=[StreamingStdOutCallbackHandler()]
)
```
- ChatModel의 생성자 중
  - streaming = True
    -  model의 응답(response)이 생성되는 것을 볼 수 있음. 응답이 끝날때까지 기다리기만하는 것이 아닌.
    - 작업이 끝날때까지 기다리지않고, 응답의 생성 과정을 볼 수 있음 
  - callbakcs 
    - StreamingStdOutCallbackHandler 
      - 응답 생성을 콘솔에서 확인 가능
      - 볼 수 있는 문자가 있을 떄마다 콘솔에 출력해줌
	- callback은 LLM에 일어나는 events를 감지하는 쉬운 방법 중 하나
    -  callback 커스텀 가능


## 3.5 Recap
- RunnableMap : LangChain 언어
  - 두번째 chain의 input을 첫번째 chain의 결과값으로 지정해준 부분
  - `|` 앞의 응답 결과를 다음 요소로 전달해주는 것

---

# Chapter4

LangChain의 모듈

모델 I/O
- 입력(input) : prompt(명령 등을 내리는 곳)
- 출력(output)

- retrieval : 외부 데이터로 접근하여 이를 모델에 어떻게 제공하는 것에 관한 것

## 4.1 FewShowPromptTemplate
템플릿 생성의 좋은점
- 유효성 검사 가능 
- prompt template을 디스크에 저장하고 load 할 수 있음 (추후 알아볼 것)

<br>

- `Fewshot` : 모델에게 예제를 준다
  - prompt로 전달하는 것보다 내가 원하는 것을 예제로 보여주는 것이 더 효과적
- 예를 들어 고객 지원봇을 만든다고 했을 때, 과거의 이력들을 fewshot으로 제공한다면 효과적인 응답을 얻을 수 있음. 이때, 데이터베이스에 저장된 과거 이력들을 이용할 수 있음

```python
from langchain.prompts import PromptTemplate
from langchain.prompts.few_show import FewShowPromptTemplate

# examples = [
# 	{
# 		"question": "",
# 		"answer": ""
# 	}
# 	# ,... 예제 리스트
# ]

examples = [
    {
        "question": "What do you know about France?",
        "answer": """
        Here is what I know:
        Capital: Paris
        Language: French
        Food: Wine and Cheese
        Currency: Euro
        """,
    },
    {
        "question": "What do you know about Italy?",
        "answer": """
        I know this:
        Capital: Rome
        Language: Italian
        Food: Pizza and Pasta
        Currency: Euro
        """,
    },
    {
        "question": "What do you know about Greece?",
        "answer": """
        I know this:
        Capital: Athens
        Language: Greek
        Food: Souvlaki and Feta Cheese
        Currency: Euro
        """,
    },
]

example_templates = """
	Human: {question}
	AI: {answer}
"""
example_prompt = PromptTemplate.from_tempate(example_templates)

prompt = FewShowPromptTemplate(
	example_prompt=example_prompt,
	examples=examples,
	suffix="Human: What do you know about {country}?",
	input_variables=["country"]
)

prompt.format(country="Germany")

```

- 위와 같이 하면, LangChain이 예제 리스트들을 위 prompt를 사용하여 형식화해줌

## 4.2 FewShotChatMessagePromptTemplate

```python
from langchain.prompts import ChatPromptTemplate
from langchain.prompts.few_show import FewShowChatMessagePromptTemplate

examples = [
    {
        "country": "France",
        "answer": """
        Here is what I know:
        Capital: Paris
        Language: French
        Food: Wine and Cheese
        Currency: Euro
        """,
    },
    {
        "country": "Italy",
        "answer": """
        I know this:
        Capital: Rome
        Language: Italian
        Food: Pizza and Pasta
        Currency: Euro
        """,
    },
    {
        "country": "Greece",
        "answer": """
        I know this:
        Capital: Athens
        Language: Greek
        Food: Souvlaki and Feta Cheese
        Currency: Euro
        """,
    },
]

example_prompt = ChatPromptTemplate.from_message([
	("human", "What do you know about {question}?"),
	("ai", "{answer}")
])

example_prompt = FewShowChatMessagePromptTemplate
(
	example_prompt=example_prompt,
	examples=examples,
)

final_prompt = ChatPromptTemplate.from_messages([
	("system", "You are a geography expert"),
	example_prompt,
	("human", "What do you know about {country}?")
])
```

- 모든 예제를 모델에게 줄 수 없음. 
- 예제를 선택해야 함. 
- 왜? 
  1. 비용이 많이 듦. 많은 텍스트
  2. 허용하는 범위 존재. 

