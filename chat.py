import os
import google.generativeai as genai

def generate_response(user_input, chat_history):
    # Configure API Key
    genai.configure(api_key="AIzaSyAtJlXxpcXr-2O7IpcCiTEBCvGj0ehJefM")
    
    model = genai.GenerativeModel("gemini-2.0-flash")
    
    instruction = """
    You are a virtual assistant dedicated exclusively to women's safety. 
    You provide users with information, resources, and support related to 
    personal safety, emergency contacts, self-defense tips, and local support organizations.
    If a user asks a question unrelated to women's safety, you must politely decline by saying:
    'I'm here to assist with women's safety. Please ask me about personal security, emergency help, 
    or self-defense tips.' Do not answer general knowledge, entertainment, or other unrelated queries. 
    Answer in numbered bullet points. Don't format the font of titles in answers.
    """
    
    contents = [{"role": "user", "parts": [{"text": instruction}]}] + chat_history + [
        {"role": "user", "parts": [{"text": user_input}]}
    ]
    
    generate_content_config = {
        "temperature": 2,
        "top_p": 0.95,
        "top_k": 40,
        "max_output_tokens": 8192,
        "response_mime_type": "text/plain"
    }
    
    response = model.generate_content(
        contents=contents, generation_config=generate_content_config
    )
    
    return response.text.strip()

def chatbot():
    print("\nWomen's Safety Chatbot - Type 'exit' to quit\n")
    chat_history = []
    while True:
        user_input = input("You: ")
        if user_input.lower() in ["exit", "quit"]:
            print("Goodbye! Stay safe.")
            break
        
        response = generate_response(user_input, chat_history)
        print("Bot:\n")
        print(response, "\n")

        chat_history.append({"role": "user", "parts": [{"text": user_input}]})
        chat_history.append({"role": "model", "parts": [{"text": response}]})

if __name__ == "__main__":
    chatbot()
