## Pakistan Culture & Linguistic Simulator

A console-based C++ application that lets users learn Pakistani regional languages and explore the rich cultural heritage of Pakistan, all in one interactive system.

## About the Project

This project was built as an Object-Oriented Programming semester project. It simulates a language-learning and culture-exploration platform covering 7 Pakistani languages — Punjabi, Urdu, Sindhi, Balochi, Pashto, Kashmiri, and Gilgiti, along with their associated cultural traditions, proverbs, taboos, cuisine, and historical figures.The system is designed around core OOP principles including abstraction, encapsulation, inheritance, and polymorphism, with a full user authentication system, persistent file-based data storage, and a gamified learning experience.

## Features

🔐 User Authentication — Sign up and login system with email validation and password confirmation

📚 Language Learning Modules — Beginner, Intermediate, and Advanced vocabulary modules for all 7 languages

🌍 Culture Explorer — Explore proverbs, cultural taboos, traditional foods, domain terms, and notable historical roles for each culture

❓ Multi-level Quizzes — Level-locked quizzes with scoring, explanations, and automatic weak area detection

🎮 Mini Games — Hangman and Word Match games with Punjabi vocabulary

🏆 Global Leaderboard — Persistent leaderboard ranking all users by score

🔥 Streak System — Daily login streak tracking with milestone badges (10, 30, 60, 365 days)

🏅 Badge System — Earn badges for completing lessons, acing quizzes, leveling up, and more

📈 Progress Tracking — Visual progress bar, mastered words counter, and weak area identification

✍️ Custom Culture Aspects — Users can contribute their own cultural knowledge to the system

🔊 Pronunciation Guide — Phonetic breakdown of key Punjabi words and phrases

💾 Persistent Storage — All user data, scores, streaks, and leaderboard saved to local .dat files


## OOP Concepts Used

Abstract Classes --- Language, Culture, Game base classes

Inheritance --- Punjabi, Urdu, Sindhi etc. inherit from Language; all cultures inherit from Culture

Polymorphism --- Language pointer array dispatches correct methods at runtime

Encapsulation --- User class with private data and public getters/setters

Static Members --- FileHandler utility class, getCurrentDate()

Constructor & Initializer List --- Used across all classes

File I/O --- ifstream, ofstream, stringstream for persistent data

STL Containers --- vector, map, set, pair used throughout


## Tech Stack 

Language: C++

Compiler: g++ (MinGW / GCC)

IDE: Visual Studio Code

Storage: File-based persistence (.dat files)

Libraries: STL (iostream, fstream, sstream, vector, map, set, algorithm, ctime)


## Team Members

Aamina Khalid

Ezza Batool

Mahrukh Khan

## Department of Computer Science, IST Islamabad
