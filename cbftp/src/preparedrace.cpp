#include "preparedrace.h"

PreparedRace::PreparedRace(unsigned int id, const std::string& release, const std::string& section, const std::list<std::pair<std::string, bool>>& sites, bool reset, int ttl) :
  id(id),
  release(release),
  section(section),
  sites(sites),
  reset(reset),
  ttl(ttl) {

}

unsigned int PreparedRace::getId() const {
  return id;
}

const std::string& PreparedRace::getRelease() const {
  return release;
}

const std::string& PreparedRace::getSection() const {
  return section;
}

bool PreparedRace::getReset() const {
  return reset;
}

const std::list<std::pair<std::string, bool>>& PreparedRace::getSites() const {
  return sites;
}

int PreparedRace::getRemainingTime() const {
  return ttl;
}

void PreparedRace::tick() {
  if (ttl >= 0) {
    ttl -= 1;
  }
}
