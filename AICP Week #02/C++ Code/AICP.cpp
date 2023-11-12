#include <iostream>
#include <iomanip>
#include <vector>

/* Program Can be stopped Early but will end when all journey tickets are purchased */
const int NUM_JOURNEYS = 4;
const int NUM_COACHES = 6;
const int SEATS_PER_COACH = 80;
const int EXTRA_COACHES_LAST_TRAIN_DOWN = 2;
const int TICKET_PRICE = 25;
const int GROUP_DISCOUNT_THRESHOLD = 10;
const int FREE_TICKET_EVERY = 10;

struct Journey {
    int passengers_count;
    int money_taken;
};

std::vector<Journey> up_journeys(NUM_JOURNEYS);
std::vector<Journey> down_journeys(NUM_JOURNEYS + 1);  // Extra journey for the last train down

void initialize_data();
void display_initial_screen();
bool purchase_tickets(int journey_index, int num_tickets);
bool purchase_return_tickets(int journey_index, int num_tickets);
void display_updated_screen();
void end_of_the_day();
void book_tickets_interactively();

int main() {
    initialize_data();
	display_initial_screen();
    book_tickets_interactively();
    end_of_the_day();

    return 0;
}

void book_tickets_interactively() {
    bool tickets_available = true;

    while (tickets_available) {
        int num_passengers, selected_departure_journey, selected_return_journey;
        char ticket_option;

        std::cout << "Enter the number of passengers (or enter 0 to end the program): ";
        std::cin >> num_passengers;

        if (num_passengers == 0) {
            break;
        }

        std::cout << "Choose ticket option (D: Departure, R: Return, B: Both): ";
        std::cin >> ticket_option;

        switch (ticket_option) {
            case 'D':
            case 'd':
                std::cout << "Enter the departure journey number (1 to " << NUM_JOURNEYS << "): ";
                std::cin >> selected_departure_journey;

                purchase_tickets(selected_departure_journey - 1, num_passengers);
                break;

            case 'R':
            case 'r':
                std::cout << "Enter the return journey number (1 to " << NUM_JOURNEYS << "): ";
                std::cin >> selected_return_journey;

                purchase_return_tickets(selected_return_journey - 1, num_passengers);
                break;

            case 'B':
            case 'b':
                std::cout << "Enter the departure journey number (1 to " << NUM_JOURNEYS << "): ";
                std::cin >> selected_departure_journey;

                purchase_tickets(selected_departure_journey - 1, num_passengers);

                std::cout << "Enter the return journey number (1 to " << NUM_JOURNEYS << "): ";
                std::cin >> selected_return_journey;

                purchase_return_tickets(selected_return_journey - 1, num_passengers);
                break;

            default:
                std::cout << "Invalid option. Please enter D, R, or B." << std::endl;
                continue;
        }

        // Display the updated screen after the purchases
        display_updated_screen();

        // Check if any tickets are still available
        tickets_available = false;

        // Check for available tickets in each journey
        for (int i = 0; i < NUM_JOURNEYS; ++i) {
            int available_tickets_up = NUM_COACHES * SEATS_PER_COACH - up_journeys[i].passengers_count;
            int available_tickets_down = (i == NUM_JOURNEYS - 1)
                                             ? (NUM_COACHES + EXTRA_COACHES_LAST_TRAIN_DOWN) * SEATS_PER_COACH - down_journeys[i].passengers_count
                                             : NUM_COACHES * SEATS_PER_COACH - down_journeys[i].passengers_count;

            if (available_tickets_up > 0 || available_tickets_down > 0) {
                tickets_available = true;
                break;
            }
        }
    }
}


void initialize_data() {
    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        up_journeys[i].passengers_count = 0;
        up_journeys[i].money_taken = 0;

        down_journeys[i].passengers_count = 0;
        down_journeys[i].money_taken = 0;
    }

    // Initialize the last train from the top with two extra coaches
    down_journeys[NUM_JOURNEYS].passengers_count = 0;
    down_journeys[NUM_JOURNEYS].money_taken = 0;
}

void display_initial_screen() {
    std::cout << "Electric Mountain Railway - Start of the Day" << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "Journey\t| Departure Time | Return Time | Available Tickets | Up - Total Passengers | Up - Total Money Taken | Down - Total Passengers | Down - Total Money Taken |" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        int available_tickets_up = NUM_COACHES * SEATS_PER_COACH - up_journeys[i].passengers_count;
        int available_tickets_down = (i == NUM_JOURNEYS - 1) ? (NUM_COACHES + EXTRA_COACHES_LAST_TRAIN_DOWN) * SEATS_PER_COACH - down_journeys[i].passengers_count
                                                             : NUM_COACHES * SEATS_PER_COACH - down_journeys[i].passengers_count;

        std::cout << std::setw(2) << i + 1 << "\t| " << 9 + i * 2 << ":00\t      | " << 10 + i * 2 << ":00\t  | ";
        
        if (available_tickets_up <= 0) {
            std::cout << std::setw(18) << "Closed";
        } else {
            std::cout << std::setw(18) << available_tickets_up;
        }

        std::cout << "| " << std::setw(21) << up_journeys[i].passengers_count
                  << "| $" << std::setw(22) << up_journeys[i].money_taken << "| ";

        if (available_tickets_down <= 0) {
            std::cout << std::setw(22) << "Closed";
        } else {
            std::cout << std::setw(22) << available_tickets_down;
        }

        std::cout << "| " << std::setw(23) << down_journeys[i].passengers_count
                  << "| $" << std::setw(23) << down_journeys[i].money_taken << "|" << std::endl;
    }

    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

bool purchase_tickets(int journey_index, int num_tickets) {
    if (journey_index < 0 || journey_index >= NUM_JOURNEYS) {
        std::cout << "Invalid journey index." << std::endl;
        return false;
    }

    int available_tickets_up = NUM_COACHES * SEATS_PER_COACH - up_journeys[journey_index].passengers_count;

    if (available_tickets_up <= 0) {
        std::cout << "Train is full. Purchase not possible." << std::endl;
        return false;
    }

    if (num_tickets > available_tickets_up) {
        std::cout << "Not enough tickets available for the selected journey." << std::endl;
        return false;
    }

    // Calculate the total price
    int total_price = num_tickets * TICKET_PRICE;

    // Apply group discount if applicable
    if (num_tickets >= GROUP_DISCOUNT_THRESHOLD) {
        int num_free_tickets = num_tickets / FREE_TICKET_EVERY;
        total_price -= num_free_tickets * TICKET_PRICE;
    }

    // Update data for the departure journey only
    up_journeys[journey_index].passengers_count += num_tickets;
    up_journeys[journey_index].money_taken += total_price;

    std::cout << "Tickets purchased successfully!" << std::endl;
    return true;
}

bool purchase_return_tickets(int journey_index, int num_tickets) {
    if (journey_index < 0 || journey_index >= NUM_JOURNEYS) {
        std::cout << "Invalid journey index." << std::endl;
        return false;
    }

    int available_tickets_down = (journey_index == NUM_JOURNEYS - 1)
                                    ? (NUM_COACHES + EXTRA_COACHES_LAST_TRAIN_DOWN) * SEATS_PER_COACH - down_journeys[journey_index].passengers_count
                                    : NUM_COACHES * SEATS_PER_COACH - down_journeys[journey_index].passengers_count;

    if (available_tickets_down <= 0) {
        std::cout << "Train is full. Purchase not possible." << std::endl;
        return false;
    }

    if (num_tickets > available_tickets_down) {
        std::cout << "Not enough tickets available for the selected journey." << std::endl;
        return false;
    }

    // Calculate the total price
    int total_price = num_tickets * TICKET_PRICE;

    // Apply group discount if applicable
    if (num_tickets >= GROUP_DISCOUNT_THRESHOLD) {
        int num_free_tickets = num_tickets / FREE_TICKET_EVERY;
        total_price -= num_free_tickets * TICKET_PRICE;
    }

    // Update data
    down_journeys[journey_index].passengers_count += num_tickets;
    down_journeys[journey_index].money_taken += total_price;

    std::cout << "Return tickets purchased successfully!" << std::endl;
    return true;
}

void display_updated_screen() {
    std::cout << "\nElectric Mountain Railway - Updated Screen" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Journey\t| Departure Time | Return Time | Available Tickets | Up - Total Passengers | Up - Total Money Taken | Down - Total Passengers | Down - Total Money Taken |" << std::endl;
    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        int available_tickets_up = NUM_COACHES * SEATS_PER_COACH - up_journeys[i].passengers_count;
        int available_tickets_down = (i == NUM_JOURNEYS - 1) ? (NUM_COACHES + EXTRA_COACHES_LAST_TRAIN_DOWN) * SEATS_PER_COACH - down_journeys[i].passengers_count
                                                             : NUM_COACHES * SEATS_PER_COACH - down_journeys[i].passengers_count;

        std::cout << std::setw(2) << i + 1 << "\t| " << 9 + i * 2 << ":00\t      | " << 10 + i * 2 << ":00\t  | ";

        if (available_tickets_up <= 0) {
            std::cout << std::setw(18) << "Closed";
        } else {
            std::cout << std::setw(18) << available_tickets_up;
        }

        std::cout << "| " << std::setw(21) << up_journeys[i].passengers_count
                  << "| $" << std::setw(22) << up_journeys[i].money_taken << "| ";

        if (available_tickets_down <= 0) {
            std::cout << std::setw(22) << "Closed";
        } else {
            std::cout << std::setw(22) << available_tickets_down;
        }

        std::cout << "| " << std::setw(23) << down_journeys[i].passengers_count
                  << "| $" << std::setw(23) << down_journeys[i].money_taken << "|" << std::endl;
    }

    std::cout << "--------------------------------------------------------------------------------------------------------------------------------------------------------------" << std::endl;
}

void end_of_the_day() {
    std::cout << "\nEnd of the Day Statistics" << std::endl;
    std::cout << "=========================" << std::endl;

    // Display statistics for up journeys
    std::cout << "Up Journeys:" << std::endl;
    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        std::cout << "Journey " << i + 1 << ": " << up_journeys[i].passengers_count << " passengers, $"
                  << up_journeys[i].money_taken << " taken" << std::endl;
    }

    // Display statistics for down journeys
    std::cout << "\nDown Journeys:" << std::endl;
    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        std::cout << "Journey " << i + 1 << ": " << down_journeys[i].passengers_count << " passengers, $"
                  << down_journeys[i].money_taken << " taken" << std::endl;
    }

    // Find the journey with the most passengers
    int max_passengers_up = 0;
    int max_passengers_down = 0;
    int journey_max_passengers_up = 0;
    int journey_max_passengers_down = 0;

    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        if (up_journeys[i].passengers_count > max_passengers_up) {
            max_passengers_up = up_journeys[i].passengers_count;
            journey_max_passengers_up = i + 1;
        }

        if (down_journeys[i].passengers_count > max_passengers_down) {
            max_passengers_down = down_journeys[i].passengers_count;
            journey_max_passengers_down = i + 1;
        }
    }

    // Display overall statistics
    int total_passengers = 0;
    int total_money_taken = 0;

    for (int i = 0; i < NUM_JOURNEYS; ++i) {
        total_passengers += up_journeys[i].passengers_count + down_journeys[i].passengers_count;
        total_money_taken += up_journeys[i].money_taken + down_journeys[i].money_taken;
    }

    std::cout << "\nOverall Statistics:" << std::endl;
    std::cout << "Total Passengers: " << total_passengers << std::endl;
    std::cout << "Total Money Taken: $" << total_money_taken << std::endl;
    std::cout << "Journey with the Most Passengers (Up): Journey " << journey_max_passengers_up << " with " << max_passengers_up << " passengers" << std::endl;
    std::cout << "Journey with the Most Passengers (Down): Journey " << journey_max_passengers_down << " with " << max_passengers_down << " passengers" << std::endl;
}

