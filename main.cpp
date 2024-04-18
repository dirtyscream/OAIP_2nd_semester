#include <fstream>
#include <iostream>
#include <vector>

class Human {
   protected:
    std::string full_name_;
    std::string date_of_birth_;

   public:
    virtual ~Human() = default;

    void set_full_name(const std::string& full_name) { full_name_ = full_name; }

    std::string get_full_name() const { return full_name_; }

    void set_date_of_birth(const std::string& date_of_birth) {
        date_of_birth_ = date_of_birth;
    }

    std::string get_date_of_birth() const { return date_of_birth_; }
};

class Teacher : public Human {
   protected:
    std::string position_;
    std::string academic_degree_;
    std::string specialty_;
    std::vector<std::string> publications_;

   public:
    void set_position(const std::string& position) { position_ = position; }

    std::string get_position() const { return position_; }

    void set_academic_degree(const std::string& academic_degree) {
        academic_degree_ = academic_degree;
    }

    std::string get_academic_degree() const { return academic_degree_; }

    void set_specialty(const std::string& specialty) { specialty_ = specialty; }

    std::string get_specialty() const { return specialty_; }

    void add_publication(const std::string& publication) {
        publications_.push_back(publication);
    }

    std::vector<std::string> get_publications() const { return publications_; }
};

class Representative : public Human {
   protected:
    std::string commission_name_;
    int year_appointed_;

   public:
    void set_commission_name(const std::string& commission_name) {
        commission_name_ = commission_name;
    }

    std::string get_commission_name() const { return commission_name_; }

    void set_year_appointed(int year_appointed) {
        year_appointed_ = year_appointed;
    }

    int get_year_appointed() const { return year_appointed_; }
};

class TeacherRepresentative : public Teacher, public Representative {
   protected:
    std::vector<std::string> works_done_;

   public:
    void add_work_done(const std::string& work_done) {
        works_done_.push_back(work_done);
    }

    std::vector<std::string> get_works_done() const { return works_done_; }
};

int main() {
    std::vector<Human*> people;

    Teacher teacher1;
    teacher1.set_full_name("John Doe");
    teacher1.set_date_of_birth("01/01/1970");
    teacher1.set_position("Professor");
    teacher1.set_academic_degree("PhD");
    teacher1.set_specialty("Computer Science");
    teacher1.add_publication("Paper 1");
    teacher1.add_publication("Paper 2");
    people.push_back(&teacher1);

    Teacher teacher2;
    teacher2.set_full_name("Jane Smith");
    teacher2.set_date_of_birth("02/02/1980");
    teacher2.set_position("Associate Professor");
    teacher2.set_academic_degree("PhD");
    teacher2.set_specialty("Physics");
    teacher2.add_publication("Paper 3");
    people.push_back(&teacher2);

    std::ofstream outfile("teachers.txt");
    if (outfile.is_open()) {
        for (const auto& person : people) {
            auto teacher = dynamic_cast<Teacher*>(person);
            if (teacher) {
                if (teacher->get_academic_degree() == "PhD") {
                    outfile << "Full Name: " << teacher->get_full_name()
                            << std::endl;
                    outfile << "Date of Birth: " << teacher->get_date_of_birth()
                            << std::endl;
                    outfile << "Position: " << teacher->get_position()
                            << std::endl;
                    outfile
                        << "Academic Degree: " << teacher->get_academic_degree()
                        << std::endl;
                    outfile << "Specialty: " << teacher->get_specialty()
                            << std::endl;
                    outfile << "Publications:" << std::endl;
                    for (const auto& publication :
                         teacher->get_publications()) {
                        outfile << "- " << publication << std::endl;
                    }
                    outfile << std::endl;
                }
            } else {
                auto human = dynamic_cast<Human*>(person);
                if (human) {
                    outfile << "Full Name: " << human->get_full_name()
                            << std::endl;
                    outfile << "Date of Birth: " << human->get_date_of_birth()
                            << std::endl;
                    outfile << std::endl;
                }
            }
        }
        outfile.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }

    return 0;
}
