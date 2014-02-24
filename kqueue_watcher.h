#ifndef FSW_KQUEUE_WATCHER_H
#define FSW_KQUEUE_WATCHER_H

#include "config.h"

#ifdef HAVE_SYS_EVENT_H

#include "watcher.h"
#include "fsw_map.h"
#include <string>
#include <vector>
#include <sys/stat.h>

using namespace std;

class kqueue_watcher: public watcher
{
public:
  kqueue_watcher(vector<string> paths, EVENT_CALLBACK callback);
  virtual ~kqueue_watcher();

  void run();

private:
  kqueue_watcher(const kqueue_watcher& orig);
  kqueue_watcher& operator=(const kqueue_watcher & that);

  void initialize_kqueue();
  bool watch_path(const string &path);
  bool add_watch(const string & path, int & descriptor, mode_t & mode);
  void remove_watch(const string &path);
  void remove_watch(int fd);
  bool is_path_watched(const string & path);
  void remove_deleted();
  void rescan_pending();
  void scan_root_paths();
  int wait_for_events(
      const vector<struct kevent> &changes,
      vector<struct kevent> &event_list);
  void process_events(
      const vector<struct kevent> &changes,
      const vector<struct kevent> &event_list,
      int event_num);

  int kq = -1;
  // initial load
  fsw_hash_map<string, int> descriptors_by_file_name;
  fsw_hash_map<int, string> file_names_by_descriptor;
  fsw_hash_map<int, bool> descriptors_to_remove;
  fsw_hash_map<int, bool> descriptors_to_rescan;
  fsw_hash_map<int, mode_t> file_modes;

  static const unsigned int MIN_SPIN_LATENCY = 1;
};

#endif  /* HAVE_SYS_EVENT_H */
#endif  /* FSW_KQUEUE_WATCHER_H */
