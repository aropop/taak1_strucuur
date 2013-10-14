//============================================================================
// Name        : Taak1.cpp
// Author      : Arno De Witte
// Version     : 1
// Description : Taak 1, Structuur 2
//============================================================================

#include <iostream> //bestand lezen en schrijven
#include <stdlib.h> //exit
#include <string.h> //to_string
#include <fstream> //bestand streams
#include <sstream> //string streams
//ondersteunde versie
const int VERSION = 1;

//declaraties
void print(std::string error_message, bool add_endl = true);
void print(int i, bool add_endl = true);
void print_choice(std::string choice, int choice_number);
void print_question(std::string question, int question_number);
void print_choice_sentence(int max);
void handle_output(std::string answer, int question_number, std::ofstream& os);
int handle_header(std::ifstream &is, std::ofstream &os);

void handle_questions(std::string input_file_name,
		std::string output_file_name) {
	//laad de file als input stream
	std::ifstream input_file_stream(input_file_name.c_str());
	std::ofstream output_file_stream(output_file_name.c_str());
	std::ofstream& ofl(output_file_stream);
	std::ifstream& ifl(input_file_stream);

	int amount_of_steps;
	amount_of_steps = handle_header(ifl, ofl);

	int count(1);
	//zolang er nog vragen zijn gaan we voort
	while (!input_file_stream.eof()) {

		//lees de huidige lijn uit en maak er een string stream van
		std::string current_line;
		getline(input_file_stream, current_line);
		std::stringstream line_stream(current_line);

		//vraag nummer uitlezen
		int question_number;
		line_stream >> question_number;

		//kijken of de vraagnummers nog kloppen
		if (count != question_number || count > amount_of_steps) {

			//nummer klopt niet met de type
			print("Input bestand is niet correct, vraag nummers kloppen niet");
			exit(1);
		} else {

			//counter ophogen
			count++;

			//type vraag uitlezen
			std::string type_of_question;
			line_stream >> type_of_question;

			std::string current_question, answer;

			if (type_of_question.compare("TEXT") == 0) {

				//de vraag is de rest van de string stream
				getline(line_stream, current_question);

				//vraag uitprinten
				print_question(current_question, question_number);

				//antwoord uitlezen van gebruiker
				getline(std::cin, answer);
				//blijven nagaan of het antwoord niet leeg is
				while (answer.empty()) {
					print("Geef een niet leeg antwoord!");
					getline(std::cin, answer);
				}
				handle_output(answer, question_number, ofl);

			} else if (type_of_question.compare("CHOICE") == 0) {
				//meerkeuzevraag
				int amount_of_answers;
				line_stream >> amount_of_answers;

				//lees de vraag
				getline(line_stream, current_question);

				//print vraag
				print_question(current_question, question_number);
				print_choice_sentence(amount_of_answers);

				//antwoorden printen
				for (int i = 1; i <= amount_of_answers; ++i) {
					std::string choice;
					getline(input_file_stream, choice);
					print_choice(choice, i);
				}

				//antwoord van gebruiker inlezen & verwerken
				getline(std::cin, answer);
				int answer_int = answer.c_str()[0] - 48;
				int amount_of_digits = strlen(answer.c_str());
				while ((amount_of_digits != 1)
						|| (!std::isdigit(answer.c_str()[0])
								|| !((0 < answer_int)
										&& (answer_int <= amount_of_answers)))) {
					print("Geef een correct getal in!");
					getline(std::cin, answer);
					answer_int = answer.c_str()[0] - 48;
					amount_of_digits = strlen(answer.c_str());
				}
				handle_output(answer, question_number, ofl);

			} else {
				print(
						"Input bestand is niet correct, ongekend current_question soort");
				exit(1);
			}

			print("OK!");
		}
	}

	//afsluitende string printen
	print("");
	print("Bedankt voor je deelname.");
	print(
			std::string("Antwoordbestand weggeschreven naar \"")
					+ output_file_name + std::string("\"."));
}

void handle_output(std::string answer, int question_number,
		std::ofstream& output_stream) {
	output_stream << question_number << " " << answer << std::endl;
}

int handle_header(std::ifstream &input_stream, std::ofstream &output_stream) {
	//variabelen declareren
	std::string current_line, current_identifier;
	int version;
	std::stringstream ss;

	//versie uitlezen
	getline(input_stream, current_line);
	ss.str(current_line);
	ss >> current_identifier;
	ss >> version;

	if (current_identifier.compare("VERSION")) {
		//identifier is niet VERSION
		print(
				std::string(
						"Input bestand incorrect, verwacht 'VERSION' gegeven '")
						+ std::string(current_identifier) + std::string("'"));
		exit(1);
	} else if (version != VERSION) {
		//de versie is niet ondersteunt
		print(
				std::string(
						"Input bestand incorrect, niet ondersteund versie nummer '"),
				false);
		print(version, false);
		print(std::string("'"));
		exit(1);

	} else {
		//volgende lijn uitlezen
		current_line.clear();
		current_identifier.clear();
		getline(input_stream, current_line);
		ss.clear();
		ss.str(current_line);

		//id uitlezen
		int id;
		ss >> current_identifier;
		ss >> id;

		if (current_identifier.compare(std::string("ID")) != 0) {
			//identifier is niet ID
			print(
					std::string(
							"Input bestand incorrect, verwacht 'ID' gekregen '")
							+ std::string(current_identifier)
							+ std::string("'"));
			exit(1);

		} else {

			//schrijf het id weg
			output_stream << current_line << std::endl;

			//lees steps uit
			current_line.clear();
			current_identifier.clear();
			ss.clear();
			getline(input_stream, current_line);
			ss.str(current_line);
			ss >> current_identifier;
			int steps;
			ss >> steps;

			if (current_identifier.compare(std::string("STEPS")) != 0) {
				//identifier is niet STEPS
				print(
						std::string(
								"Input bestand incorrect, verwacht 'STEPS' gekregen '")
								+ current_identifier + std::string("'"));
				exit(1);
			} else {
				//print de zin die zegt hoeveel vragen er zijn
				print(std::string("Deze enquete bestaat uit "), false);
				print(steps, false);
				print(std::string(" vragen."));
				//print een enter
				print(std::string(""));
				return steps;
			}

		}
	}
	//kan hier niet komen maar voorkomt een foutmelding van de compiler
	return 0;

}

void print(std::string error_message, bool add_endl) {
	std::cout << error_message;
	if (add_endl) {
		std::cout << std::endl;
	}

}
//overloaden om ints te supporten
void print(int i, bool add_endl) {
	std::cout << i;
	if (add_endl) {
		std::cout << std::endl;
	}
}

/*
 * Functie die een vraag uitprint
 */
void print_question(std::string question, int question_number) {
	std::cout << "Vraag " << question_number << ": " << question << std::endl;
}

void print_choice(std::string choice, int choice_num) {
	std::cout << choice_num << ") " << choice << std::endl;
}

void print_choice_sentence(int choices) {
	std::cout << "Maak een keuze uit: (1-" << choices << ")" << std::endl;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		print("Niet genoeg argumenten");
		exit(1);
	}
	std::string input_file_name, output_file_name;
	input_file_name = argv[1]; //we verwachten dat het eerste argument de input file is
	output_file_name = argv[2]; //2de argument is de output file

	handle_questions(input_file_name, output_file_name);

	return 0;
}
