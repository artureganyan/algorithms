// Problem: https://leetcode.com/problems/reconstruct-itinerary/

#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include "utils.h"

namespace reconstruct_itinerary {

class Solution {
public:
    // Note: Each ticket must be a vector of 2 strings: {from, to}. The strings
    // are not checked, although the original problem supposes only the strings
    // of 3 capital letters. The tickets can repeat. The tickets with from == to
    // or with empty names are accepted. If there is no path which uses each
    // ticket, returns an empty result. If there are multiple pathes, returns
    // the one which goes first alphabetically.
    //
    std::vector<std::string> run(const std::vector<std::vector<std::string>>& tickets)
    {
        if (!tickets.size())
            return {};

        // Construct the tickets map. Tickets are sorted by the destination, in
        // the alphabetical order.
        typedef std::unordered_map<std::string /*from*/, std::map<std::string /*to*/, int /*tickets*/>> TicketsMap;

        TicketsMap tickets_map;
        for (const auto& ticket : tickets)
            tickets_map[ticket[0]][ticket[1]]++;

        // Find the path that uses each ticket, via DFS
        typedef std::unordered_map<std::string /*from*/, std::unordered_map<std::string /*to*/, int /*tickets*/>> UsedTicketsMap;

        struct NextStation
        {
            std::string name;
            int         path_size;
        };

        const char* START_STATION = "JFK";

        std::vector<NextStation> next_stations = {{START_STATION, 0}};
        std::vector<std::string> path;
        UsedTicketsMap           used_tickets_map;

        while (next_stations.size()) {
            // Take the current station
            const auto station = next_stations.back();
            next_stations.pop_back();

            // Rollback the path to this station, returning used tickets
            if (station.path_size > 0) {
                for (int i = station.path_size; i < path.size(); i++)
                    used_tickets_map[path[i - 1]][path[i]]--;
            }
            path.resize(station.path_size);
            path.push_back(station.name);

            // Add next stations to the queue
            const auto& station_tickets      = tickets_map[station.name];
            auto&       used_station_tickets = used_tickets_map[station.name];
            int         new_next_stations    = 0;

            for (auto tickets = station_tickets.rbegin(); tickets != station_tickets.rend(); tickets++) {
                const auto& target      = tickets->first;
                const int   total_count = tickets->second;
                if (used_station_tickets[target] == total_count)
                    continue;

                next_stations.push_back({target, (int) path.size()});
                new_next_stations++;
            }

            // We have new next stations, so mark the next ticket as used
            if (new_next_stations > 0) {
                used_station_tickets[next_stations.back().name]++;

            // We have no next station, so stop if the path uses each ticket
            } else if (path.size() == tickets.size() + 1) {
                return path;

            // The path is finished but does not use each ticket
            } else {
                // Continue searching
            }
        }

        // The path is not found
        return {};
    }
};

int main()
{
    typedef std::vector<std::string> Result;

    ASSERT(( Solution().run({}) == Result{} ));

    ASSERT(( Solution().run({{"JFK", "JFK"}}) == Result{"JFK", "JFK"} ));
    ASSERT(( Solution().run({{"JFK", "JFK"}, {"JFK", "JFK"}}) == Result{"JFK", "JFK", "JFK"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}}) == Result{"JFK", "SFO"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "JFK"}}) == Result{"JFK", "SFO", "JFK"} ));
    ASSERT(( Solution().run({{"SFO", "JFK"}, {"JFK", "SFO"}}) == Result{"JFK", "SFO", "JFK"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "JFK"}, {"JFK", "SFO"}}) ==
             Result{"JFK", "SFO", "JFK", "SFO"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "SFO"}, {"SFO", "JFK"}, {"JFK", "JFK"}}) ==
             Result{"JFK", "JFK", "SFO", "SFO", "JFK"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}, {"JFK", "LGA"}}) == Result{} ));
    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "LGA"}}) == Result{"JFK", "SFO", "LGA"} ));
    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "LGA"}, {"SFO", "JFK"}}) == Result{} ));
    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "LGA"}, {"LGA", "JFK"}}) ==
             Result{"JFK", "SFO", "LGA", "JFK"} ));

    ASSERT(( Solution().run({{"JFK", "SFO"}, {"SFO", "JFK"}, {"JFK", "LGA"}, {"LGA", "JFK"}}) ==
             Result{"JFK", "LGA", "JFK", "SFO", "JFK"} ));

    ASSERT(( Solution().run({{"JFK", "LGA"}, {"JFK", "SFO"}, {"SFO", "JFK"}}) ==
             Result{"JFK", "SFO", "JFK", "LGA"} ));

    return 0;
}

}
