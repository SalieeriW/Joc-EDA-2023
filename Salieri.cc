#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Salieri


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<int> VID;
  typedef vector<int> Row;
  typedef vector<Row> Mat;
  typedef pair<int,int> Coord;

  struct exec_priority {
    int type;
    int priority;
    int health;
    int dir;
    int id;
  };

  struct cmp {
    bool operator()(const exec_priority& a, const exec_priority& b) {
        // Order by type in decreasing order
        if (a.type != b.type) {
            return a.type < b.type;
        }

        // In case of tie on type, order by priority in decreasing order
        if (a.priority != b.priority) {
            return a.priority < b.priority;
        }

        // In case of tie on type and priority, order by health in increasing order
        return a.health > b.health;
    }
};

  // U1. Upgradable: We can classify the ok block by the amount of free block adjacent to it.
  // this prevents route end.

  // U2. Follow mechanism, assign diferent numbers.




  void CheckAroundLVl1P (vector<int> &d, int u_id) {
    Pos p = unit(u_id).pos;
    for (int i = 0; i < 8; ++i) {
      if (not pos_ok(p + Dir(i))) d[i] = -3;
      else {
        Cell c1 = cell(p + Dir(i));
        if (c1.type != Cave) {
          d[i] = -3;
        } else if (c1.id != -1){
            if (unit(c1.id).player != me() and unit(c1.id).type != Pioneer) {
              d[i] = -2;
              if (d[(i + 1)%8] > -1) d[(i + 1)%8] = -1;
              if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
              if (d[(i + 7)%8] > -1) d[(i + 7)%8] = -1;
              if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
            } else d[i] = -3;
        } else if (d[i] >= 0 and c1.owner != me()) {
          d[i] = 1;
        }
      }
    }
  }


  void CheckAroundLVl1F (vector<int> &d, int u_id, int context, int context2) {
    Pos p = unit(u_id).pos;
    for (int i = 0; i < 8; ++i) {
      if (not pos_ok(p + Dir(i))) d[i] = -3;
      else {
        Cell c1 = cell(p + Dir(i));
        if (c1.type != Cave) d[i] = -3; 
        else if (c1.id != -1){
          if (unit(c1.id).player != me()) {
            if (unit(c1.id).type == Pioneer and d[i] != -2) {
              if (context > 14 or context2 <= 4) d[i] = 2;
              else d[i] = 3;
            } else if (unit(c1.id).type == Furyan) {
              if (unit(c1.id).health <= 25 and d[i] != -2) d[i] = 4;
              else if (d[i] != -2) {
                if (context > 14 or context2 <= 4) d[i] = 3;
                else d[i] = 2;
              }
            } else d[i] = -3;
          } else d[i] = -3;
        }
      }
    }
  }
  
  void CheckAroundLVl2 (vector<int> &d, int u_id, int PF, int context, int context2) {
    Pos p = unit(u_id).pos;
    for (int i = 0; i < 8; i++) {
      if (i %2 == 0) {
        Cell c1, c2, c3;
        if (pos_ok(p + Dir(i) + Dir((i+1)%8))){
          c2 = cell(p + Dir(i) + Dir((i+1)%8));
          if (c2.id != -1){
            if (unit(c2.id).type == Hellhound) {
              d[i] = d[(i + 1)%8] = d[(i + 7)%8] = d[(i + 2)%8] = d[(i + 6)%8] = -2;
            } else if (unit(c2.id).player != me()) {
              if (PF == 0) {
                if (unit(c2.id).type == Furyan) {
                  d[i] = d[(i + 1)%8] = -2; 
                  if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                  if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
                  if (d[(i + 7)%8] > -1) d[(i + 7)%8] = -1; 
                }
              } else {
                if (unit(c2.id).type == Pioneer) {
                  if (context > 14 or context2 <= 4) {
                    if (d[i] >= 0) d[i] = 1;
                    if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 1;
                  } else {
                    if (d[i] >= 0) d[i] = 2;
                    if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 2;
                  }
                } else if (unit(c2.id).type == Furyan) {
                  if (unit(c2.id).health < 25) {
                    if (d[i] >= 0) d[i] = 3;
                    if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 3;
                  } else if (unit(u_id).health <= 35 and unit(c2.id).health >= 40) {
                    d[i] = d[(i + 1)%8] = -2; 
                    if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                    if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
                    if (d[(i + 7)%8] > -1) d[(i + 7)%8] = -1; 
                  } else {
                    if (context > 14 or context2 <= 4) {
                      if (d[i] >= 0) d[i] = 2;
                      if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 2;
                    } else {
                      if (d[i] >= 0) d[i] = 1;
                      if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 1;
                    }
                  } 
                }
              }
            }
          }
        }
      if (pos_ok(p + Dir(i) + Dir((i+7)%8))){
        c3 = cell(p + Dir(i) + Dir((i+7)%8));
        if (c3.id != -1){
          if (unit(c3.id).type == Hellhound) {
            d[i] = d[(i + 1)%8] = d[(i + 7)%8] = d[(i + 2)%8] = d[(i + 6)%8] = -2;
          } 
          else if (unit(c3.id).player != me()) {
            if (PF == 0) {
                if (unit(c3.id).type == Furyan) {
                  d[i] = d[(i + 7)%8] = -2;
                  if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                  if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
                  if (d[(i + 1)%8] > -1) d[(i + 1)%8] = -1; 
                }
            } else {
              if (unit(c3.id).type == Pioneer) {
                if (context > 14 or context2 <= 4) {
                  if (d[i] >= 0) d[i] = 1;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 1;
                } else {
                  if (d[i] >= 0) d[i] = 2;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 2;
                }
              } else if (unit(c3.id).type == Furyan) {
                if (unit(c3.id).health < 25) {
                  if (d[i] >= 0) d[i] = 3;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 3;
                } else if (unit(u_id).health <= 35 and unit(c3.id).health >= 40){
                  d[i] = d[(i + 7)%8] = -2;
                  if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                  if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
                  if (d[(i + 1)%8] > -1) d[(i + 1)%8] = -1; 
                } else {
                  if (context > 14 or context2 <= 4) {
                    if (d[i] >= 0) d[i] = 2;
                    if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 2;
                  } else {
                    if (d[i] >= 0) d[i] = 1;
                    if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 1;
                  }
                } 
              }
            }
          }
        }
      }
      if (pos_ok(p + Dir(i) + Dir(i))) {
        c1 = cell(p + Dir(i) + Dir(i));
        if (c1.id != -1){
          if (unit(c1.id).type == Hellhound) {
            d[i] = d[(i + 1)%8] = d[(i + 7)%8] = d[(i + 2)%8] = d[(i + 6)%8] = -2;
          }
          else if (unit(c1.id).player != me()) {
            if (PF == 0) {
              if (unit(c1.id).type == Furyan) {
                d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
                if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
              }
            } else {
              if (unit(c1.id).type == Pioneer) {
                if (context > 14 or context2 <= 4) {
                  if (d[i] >= 0) d[i] = 1;
                  if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 1;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 1;
                } else {
                  if (d[i] >= 0) d[i] = 2;
                  if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 2;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 2;
                }
              } else if (unit(c1.id).type == Furyan) {
                if (unit(c1.id).health < 25) {
                  if (d[i] >= 0) d[i] = 3;
                  if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 3;
                  if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 3;
                } else if (unit(u_id).health <= 35 and unit(c1.id).health >= 40) {
                  d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
                  if (d[(i + 2)%8] > -1) d[(i + 2)%8] = -1;
                  if (d[(i + 6)%8] > -1) d[(i + 6)%8] = -1;
                } else {
                  if (context > 14 or context2 <= 4) {
                    if (d[i] >= 0) d[i] = 2;
                    if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 2;
                    if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 2;
                  } else {
                    if (d[i] >= 0) d[i] = 1;
                    if (d[(i + 1)%8] >= 0) d[(i + 1)%8] = 1;
                    if (d[(i + 7)%8] >= 0) d[(i + 7)%8] = 1;
                  }
                } 
              }
            }
          }
        }
      }
    } else {
        if (pos_ok(p + Dir(i) + Dir(i))) {
          Cell c = cell(p + Dir(i) + Dir(i));
          if (c.id != -1){
            if (unit(c.id).type == Hellhound) {
              d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
            }
            else if (unit(c.id).player != me()) {
              if (PF == 0) {
                if (unit(c.id).type == Furyan) {
                  d[i] = -2;
                  if (d[(i + 1)%8] > -1) d[(i + 1)%8] = -1;
                  if (d[(i + 7)%8] > -1) d[(i + 7)%8] = -1;
                }
              } else {
                if (unit(c.id).type == Pioneer) {
                  if (context > 14 or context2 <= 4) {
                    if (d[i] >= 0) d[i] = 1;
                  } else {
                    if (d[i] >= 0) d[i] = 2;
                  }
                } 
                else if (unit(c.id).type == Furyan) {
                  if (unit(c.id).health < 25) {
                    if (d[i] >= 0) d[i] = 3;
                  } else if (unit(u_id).health <= 35 and unit(c.id).health >= 40) {
                    d[i] = -2;
                    if (d[(i + 1)%8] > -1) d[(i + 1)%8] = -1;
                    if (d[(i + 7)%8] > -1) d[(i + 7)%8] = -1;
                  }
                  else {
                    if (context > 14 or context2 <= 4) {
                      if (d[i] >= 0) d[i] = 2;
                    } else {
                      if (d[i] >= 0) d[i] = 1;
                    }
                  } 
                }
              }
            }
          }
        }
      }
    }
  }
  void CheckAroundLvl3 (vector<int> &d, int u_id) {
    Pos p = unit(u_id).pos;
    for (int i = 0; i < 8; i++) {
      if (i %2 == 0) {
        Cell c1, c2, c3;
        if (pos_ok(p + Dir(i) + Dir(i) + Dir((i+1)%8))){
          c2 = cell(p + Dir(i) + Dir((i+1)%8)+ Dir(i));
          if (c2.id != -1 and (unit(c2.id).type == Hellhound)) {
            d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
            d[(i + 2)%8] = d[(i + 6)%8] = -1;
          }
        }
        if (pos_ok(p + Dir(i) + Dir(i) + Dir((i+7)%8))){  
          c3 = cell(p + Dir(i) + Dir((i+7)%8) + Dir(i));
          if (c3.id != -1 and (unit(c3.id).type == Hellhound)){
            d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
            d[(i + 2)%8] = d[(i + 6)%8] = -1;
          }
        }
        if (pos_ok(p + Dir(i) + Dir(i) + Dir(i))) {
          c1 = cell(p + Dir(i) + Dir(i) + Dir(i));
          if (c1.id != -1 and (unit(c1.id).type == Hellhound)) {
            d[i] = d[(i + 1)%8] = d[(i + 7)%8] = -2;
            d[(i + 2)%8] = d[(i + 6)%8] = -1;
          }
        }
      } else {
        Cell c1, c2, c3;
        if (pos_ok(p + Dir(i) + Dir(i) + Dir((i+1)%8))){
          c2 = cell(p + Dir(i) + Dir((i+1)%8) + Dir(i));
          if (c2.id != -1 and (unit(c2.id).type == Hellhound)) {
            d[(i+1)%8] = d[(i + 2)%8] = d[i] = -2;
            d[(i + 3)%8] = d[(i + 7)%8] = -1;
          }
        }
        if (pos_ok(p + Dir(i) + Dir((i+7)%8) + Dir(i))){
          c3 = cell(p + Dir(i) + Dir((i+7)%8) + Dir(i));
          if (c3.id != -1 and (unit(c3.id).type == Hellhound)){
            d[(i + 7)%8] = d[i] = d[(i + 6)%8] = -2;
            d[(i + 1)%8] = d[(i + 5)%8] = -1;
          }
        }
        if (pos_ok(p + Dir(i) + Dir(i) + Dir(i))) {
          c1 = cell(p + Dir(i) + Dir(i) + Dir(i));
          if (c1.id != -1 and (unit(c1.id).type == Hellhound)){
            d[i] = -2;
            d[(i + 1)%8] = d[(i + 7)%8] = -1;
          }
        }
      }
    }
  }
  int bfs_F(int u_id, int context, int context2) {

    int threshold;
    if (context > 14 or context2 <= 4) threshold = 0;
    else threshold = 1;

    Pos p = unit(u_id).pos;
    Mat map (40, Row(80, 81));
    map[p.i][p.j] = 0;
    vector <int> perm = random_permutation(8);
    int max_dist = 1;
    queue<pair<Pos,int>> q;
    priority_queue<pair<int,int>> res;
    
    for (int i : perm) {
      Cell c = cell(p + Dir(i));
      if (pos_ok(p + Dir(i)) and c.type != Rock) q.push({p + Dir(i), i});
      map[(p + Dir(i)).i][(p + Dir(i)).j] = 1;
    }

    while (not q.empty()) {
      pair<Pos, int> u = q.front(); q.pop();
      for (int i : perm) {
        pair<Pos, int> n_node =  {u.first + Dir(i), u.second};
        Cell c = cell(n_node.first);
        if(pos_ok(n_node.first) and c.type != Rock){
          if (map[n_node.first.i][n_node.first.j] == 81) {
            map[n_node.first.i][n_node.first.j] = map[u.first.i][u.first.j] + 1;
            if (map[n_node.first.i][n_node.first.j] > max_dist) max_dist = map[n_node.first.i][n_node.first.j];
            if (c.id != -1 and unit(c.id).player != me()) {
              if (unit(c.id).type == Pioneer) {
                if (unit(c.id).health <= 25) res.push({3 + threshold, n_node.second});
                else res.push({2 + threshold, n_node.second});
              } else if (unit(c.id).type == Furyan) {
                if (unit(c.id).health <= 25) res.push({5, n_node.second});
                else if (not (unit(u_id).health <= 25 and unit(c.id).health >= 50)) res.push({4 - threshold*2, n_node.second});
              }
            } else if (c.id == -1) q.push({u.first + Dir(i), u.second});
          }
        }
      }
      if (max_dist%5 == 0 and not res.empty()) return res.top().second;
    }
    return -1;
  }

int bfs_P(int u_id) {

  Pos p = unit(u_id).pos;
  Mat map (40, Row(80, -1));
  map[p.i][p.j] = 1;
  queue<pair<Pos,int>> q;
  vector <int> perm = random_permutation(8);

  for (int i : perm) {
    Cell c = cell(p + Dir(i));
    if (pos_ok(p + Dir(i)) and c.type != Rock and c.id == -1) {
      q.push({p + Dir(i), i});
      if (c.owner != me()) return i;
    }
  }  

  while (not q.empty()) {

    pair<Pos, int> u = q.front(); q.pop();
    for (int i : perm) {
      pair<Pos, int> n_node =  {u.first + Dir(i), u.second};
      Cell c = cell(n_node.first);

      if (pos_ok(n_node.first) and c.type != Rock and c.id == -1){
        if (map[n_node.first.i][n_node.first.j] == -1) {
          map[n_node.first.i][n_node.first.j] = 1;
          if (c.owner != me()) {
            return n_node.second;
          }
          q.push({n_node.first, u.second});
        }
      }
    }
  }
  return -1;
}
         
  void wander(int u_id, char c, priority_queue<exec_priority, vector<exec_priority>, cmp> &exec_order, int PF, int context, int context2) {
    
    int best_dir = -1;
    int level = -3;
    int priority = 0;
    vector <int> BlockAround (8, 0);
    bool search = true;

    if (c == 'P') {
      CheckAroundLVl2(BlockAround, u_id, 0, context, context2);
      CheckAroundLvl3(BlockAround, u_id);
      CheckAroundLVl1P(BlockAround, u_id);
    }
    else {
      CheckAroundLVl2(BlockAround, u_id, 1, context, context2);
      CheckAroundLvl3(BlockAround, u_id);
      CheckAroundLVl1F(BlockAround, u_id, context, context2);
    }

    for (int i = 0; i < 8; ++i) {
      if (BlockAround[i] > level or (BlockAround[i] == level and random(0,1) == 1 and level != -3)) {
        level = BlockAround[i];
        best_dir = i; // U1(29).
      }
      if ((BlockAround[i] < 0 and BlockAround[i] != -3) or BlockAround[i] > 0) search = false;

      if (BlockAround[i] == -2) {
        priority = 5;
      } else if (BlockAround[i] > priority) {
        priority = BlockAround[i];
      }
    }
    if (search and c == 'P') {
      best_dir = bfs_P(u_id);
      if (best_dir == -1) best_dir = 10;
    }
    else if (search) {
      best_dir = bfs_F(u_id, context, context2);
       if (best_dir == -1) best_dir = 10;
    }
    exec_priority temp;
    temp.type = PF;
    temp.dir = best_dir;
    temp.id = u_id;
    temp.priority = priority;
    temp.health = unit(u_id).health;
    exec_order.push(temp);
  }

  void move_furyans(priority_queue<exec_priority, vector<exec_priority>, cmp> &exec_order) {
    VID F = furyans(me());
    VID P = pioneers(me());
    int n2 = P.size();
    int n = F.size();
    for (int i = 0; i < n; ++i) {
      wander(F[i], 'F', exec_order, 1, n2, n);
    }
  }


  void move_pioneers(priority_queue<exec_priority, vector<exec_priority>, cmp> &exec_order) {
    VID P = pioneers(me());
    VID F = furyans(me());
    int n = P.size();
    int n2 = F.size();
    for (int i = 0; i < n; ++i) {
      wander(P[i], 'P', exec_order, 0, n, n2);
    }
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    priority_queue<exec_priority, vector<exec_priority>, cmp> exec_order;
    move_furyans(exec_order);
    move_pioneers(exec_order);
    while (not exec_order.empty()) {
      command(exec_order.top().id, Dir(exec_order.top().dir));
      exec_order.pop();
    }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
