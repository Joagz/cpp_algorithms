/**
 * @brief A* Pathfinding algorithm for robOs project
 * @author Joaquin Gomez
 * @date 23.01.2025 (DD/MM/YYYY)
 */
#ifndef A_STAR_ROBALGOR
#define A_STAR_ROBALGOR

#define A_STAR_BLOCKED false
#define A_STAR_BLOCKED true

/**
 * Basically each node in the map has information in each position
 * - The first byte is the state (8 bit unsigned integer)
 * - The last 2 bytes are the f_cost (16 bit unsigned integer)
 */

#define A_STAR_ERROR 0xFFFFFFFE
#define A_STAR_ERROR_16 0xEEEE

#define A_STAR_STATE_MASK 0b00000000000000000000000000000001
#define A_STAR_GCOST_MASK 0b00000000000000001111111111111110
#define A_STAR_FCOST_MASK 0b11111111111111110000000000000000
                                 
#define A_STAR_STATE_MASK_NEGATE 0b11111111111111111111111111111110
#define A_STAR_GCOST_MASK_NEGATE 0b11111111111111110000000000000001
#define A_STAR_FCOST_MASK_NEGATE 0x0000FFFF

#define A_STAR_NODE_ENABLED 0b11111111111111110000000000000001
#define A_STAR_NODE_BLOCKED 0b11111111111111110000000000000000
#define A_STAR_NODE_STARTER 0b00000000000000000000000000000001 // Starter node must have 0 f_cost

#include <cstdint>

class A_star
{
private:
    std::uint32_t **map;  // Map points
    std::uint32_t xs, ys; // Map resolution (xs*ys must be bounded to be a 32bit unsigned integer)
    /**
     *
     * px = list of output points' x coord
     * py = list of output points' y coord
     * ps = size of both lists
     * pl = current last element index (length = pl)
     * plc = current last element index of closed list (length = plc)
     */
    std::uint32_t *px, *py, ps, pl, plc;

    /***** Debugging and error checking *****/

    bool A_star::_check_map();
    bool A_star::_check_coords(std::uint32_t px, std::uint32_t py);

    /***** Memory allocation *****/

    /**
     * @brief  Allocate memory for the map
     */
    void _loadmap();

    std::uint32_t get_open_list(std::uint32_t px, std::uint32_t py);
    std::uint32_t get_closed_list(std::uint32_t px, std::uint32_t py);

    /**
     * @brief  Allocate memory for the points
     * @param  {xs} std::uint32_t : X-Resolution of the points
     * @param  {ys} std::uint32_t : Y-Resolution of the points
     */
    void _loadpnt(std::uint32_t xs, std::uint32_t ys);

    /**
     * @brief Free map memory
     */
    void _freemap();

    /**
     * @brief Free all output points
     */
    void A_star::_freepnt();

    /***** Nodes and map functions *****/

    /**
     * @brief Get cost of the point in the map
     * @param  {xs} std::uint32_t : X coordinate of the point
     * @param  {ys} std::uint32_t : Y coordinate of the point
     * @returns The f_cost of the node
     */
    std::uint16_t _getfcost(std::uint32_t px, std::uint32_t py);
    std::uint16_t _getgcost(std::uint32_t px, std::uint32_t py);

    /**
     * @brief Set cost of the point in the map
     * @param  {xs} std::uint32_t : X coordinate of the point
     * @param  {ys} std::uint32_t : Y coordinate of the point
     * @param  {f_cost} std::uint32_t : cost of the point (f_cost = g_cost + h_cost)
     */
    void _setfcost(std::uint32_t px, std::uint32_t py, std::uint16_t f_cost);
    void _setgcost(std::uint32_t px, std::uint32_t py, std::uint16_t g_cost);

    /**
     * @brief Returns true if the node is blocked, false otherwise
     * @param  {xs} std::uint32_t : X coordinate of the point
     * @param  {ys} std::uint32_t : Y coordinate of the point
     */
    bool _isblocked(std::uint32_t px, std::uint32_t py);

    /***** Min binary heap definitions *****/
    /**
     * @brief Get closed list index
     */
    std::uint32_t cl_i(std::uint32_t i);

    /**
     * Sink down a node
     * @param {pi} std::uint32_t : index of the point in the list
     */
    void _sink(std::uint32_t pi);
    void _sink_cl(std::uint32_t pi);

    /**
     * Swim up a node
     * @param {pi} std::uint32_t : index of the point in the list
     */
    void _swim(std::uint32_t pi);
    void _swim_cl(std::uint32_t pi);

    /**
     * Add a node
     * @param {px} std::uint32_t : X position of the point
     * @param {py} std::uint32_t : Y position of the point
     */
    void _add(std::uint32_t px, std::uint32_t py);
    void _add_cl(std::uint32_t px, std::uint32_t py);

    /**
     * Remove a node
     * @param {pi} std::uint32_t : index of the point in the list
     * @returns The removed node
     */
    std::uint32_t _remove(std::uint32_t pi);
    std::uint32_t _remove_cl(std::uint32_t pi);

    /**
     * Exchange two elements
     * @param  {i0} std::uint32_t : index of first element
     * @param  {i1} std::uint32_t : index of second element
     */
    void _exchange(std::uint32_t i0, std::uint32_t i1);

public:
    A_star() {};
    A_star(std::uint32_t xs, std::uint32_t ys);
    ~A_star();

    /**
     * @brief  Sets tile state (A_STAR_BLOCKED/A_STAR_UNBLOCKED)
     * @param  {px} std::uint32_t : X Position of the tile
     * @param  {py} std::uint32_t : Y Position of the tile
     */
    void toggletile(std::uint32_t px, std::uint32_t py, bool tile_state);

    /**
     * @brief  Performs A* calculation and populates A_star::px and A_star::py.
     * @param  {sx} std::uint32_t : start X position
     * @param  {sy} std::uint32_t : start Y position
     * @param  {tx} std::uint32_t : target X position
     * @param  {ty} std::uint32_t : target Y position
     */
    void run(std::uint32_t sx, std::uint32_t sy, std::uint32_t tx, std::uint32_t ty);

    /**
     * @brief  Reconstructs the path after calculation
     * @param  {tx} std::uint32_t : target X position
     * @param  {ty} std::uint32_t : target Y position
     */
    void reconstruct(std::uint32_t tx, std::uint32_t ty);
};

#endif