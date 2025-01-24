#include "a_star.hh"
#include "ioutils.hh"

#include <cstdlib>
#include <iostream>
#include <cmath>

std::uint32_t _distance(std::uint32_t x1, std::uint32_t y1, std::uint32_t x2, std::uint32_t y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

A_star::A_star(std::uint32_t xs, std::uint32_t ys)
{
    this->xs = xs;
    this->ys = ys;
    this->_loadmap();
}

A_star::~A_star()
{
    this->_freemap();
}

void A_star::toggletile(std::uint32_t px, std::uint32_t py, bool tile_state)
{
    if (!_check_map())
        return;
    if (!_check_coords(px, py))
        return;

    this->map[px][py] = tile_state;
}

std::uint32_t A_star::get_open_list(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return this->pl;

    std::uint32_t i;
    for (i = 0; i < this->pl; i++)
    {
        if (this->px[i] == px && this->py[i] == py)
            break;
    }
    return i;
}

std::uint32_t A_star::get_closed_list(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return this->pl;

    std::uint32_t i;
    for (i = 0; i < this->plc; i++)
    {
        if (this->pxc[i] == px && this->pyc[i] == py)
            break;
    }
    return i;
}

bool A_star::in_open_list(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return false;
    for (std::uint32_t i = 0; i < this->pl; i++)
    {
        if (this->px[i] == px && this->py[i] == py)
            return true;
    }
    return false;
}

bool A_star::in_closed_list(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return false;
    for (std::uint32_t i = 0; i < this->plc; i++)
    {
        if (this->pxc[i] == px && this->pyc[i] == py)
            return true;
    }
    return false;
}

bool A_star::check_node(std::uint32_t sx, std::uint32_t sy, std::uint32_t tx, std::uint32_t ty, std::uint16_t gcost)
{
    std::uint16_t new_gcost = gcost + 1;
    std::uint16_t new_fcost = _distance(sx, sy, tx, ty) + new_gcost;

    bool right_open = in_open_list(sx, sy);
    bool right_closed = in_closed_list(sx, sy);
    bool is_right_better = _getfcost(sx, sy) < new_fcost;

    return !(right_open && is_right_better) && !(right_closed && is_right_better);
}

void A_star::run(std::uint32_t sx, std::uint32_t sy, std::uint32_t tx, std::uint32_t ty)
{
    cout_debug("run", "starting path calculation");

    if (!_check_map())
        return;

    if (!_check_coords(sx, sy) || !_check_coords(tx, ty))
        return;

    if (sx == tx && sy == ty)
        return;

    cout_debug("run", "Loading a new point array");
    _loadpnt(tx - sx, ty - sy);

    _add(sx, sy);
    this->map[sx][sy] = A_STAR_NODE_STARTER;

    while (this->pl > 0)
    {
        cout_debug("run", "evaluating...");
        std::uint32_t x = this->px[0];
        std::uint32_t y = this->px[0];
        std::uint32_t el = _remove(0);
        std::uint16_t fcost = _getfcost(x, y);
        std::uint16_t gcost = _getgcost(x, y);

        if (check_node(x + 1, y, tx, ty, gcost))
            _add(x + 1, y);

        if (check_node(x - 1, y, tx, ty, gcost))
            _add(x - 1, y);

        if (check_node(x, y + 1, tx, ty, gcost))
            _add(x, y + 1);

        if (check_node(x, y - 1, tx, ty, gcost))
            _add(x, y - 1);

        if (check_node(x + 1, y + 1, tx, ty, gcost))
            _add(x + 1, y + 1);

        if (check_node(x + 1, y - 1, tx, ty, gcost))
            _add(x + 1, y - 1);

        if (check_node(x - 1, y - 1, tx, ty, gcost))
            _add(x - 1, y - 1);

        if (check_node(x - 1, y + 1, tx, ty, gcost))
            _add(x - 1, y + 1);

        _add_cl(x, y);
    }

    _freepnt();
}

bool A_star::_check_map()
{
    cout_debug("_check_map", "trying to check map");

    if (this->map == nullptr)
    {
        cout_err("_check_map", "the 'map' variable is null");
        return false;
    }
    cout_debug("_check_map", "map is correctly loaded");

    return true;
}

bool A_star::_check_coords(std::uint32_t px, std::uint32_t py)
{

    if (px * py >= this->xs * this->ys)
    {
        cout_err("_getfcost", "indices out of bounds");
        return false;
    }

    return true;
}

void A_star::_loadmap()
{
    map = static_cast<std::uint32_t **>(std::calloc(this->xs, sizeof(std::uint32_t *)));

    for (int xi = 0; xi < this->xs; xi++)
    {
        map[xi] = static_cast<std::uint32_t *>(std::calloc(this->ys, sizeof(std::uint32_t)));

        for (int yi = 0; yi < this->ys; yi++)
        {
            map[xi][yi] = A_STAR_NODE_ENABLED;
        }
    }
}

void A_star::_freemap()
{
    for (int xi = 0; xi < this->xs; xi++)
    {
        if (map[xi] != nullptr)
            free(map[xi]);
    }

    free(map);
}

void A_star::_loadpnt(std::uint32_t xs, std::uint32_t ys)
{
    this->px = static_cast<std::uint32_t *>(std::calloc(xs * ys, sizeof(std::uint32_t)));
    this->py = static_cast<std::uint32_t *>(std::calloc(xs * ys, sizeof(std::uint32_t)));
    this->pxc = static_cast<std::uint32_t *>(std::calloc(xs * ys, sizeof(std::uint32_t)));
    this->pyc = static_cast<std::uint32_t *>(std::calloc(xs * ys, sizeof(std::uint32_t)));
    this->ps = xs * ys;
    this->pl = 0;
    this->plc = 0;
}

void A_star::_freepnt()
{
    if (this->px != nullptr)
        free(this->px);
    if (this->py != nullptr)
        free(this->py);
    this->ps = 0;
}

bool A_star::_isblocked(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return false;

    return (this->map[px][py] & A_STAR_STATE_MASK) == 1;
}

std::uint16_t A_star::_getfcost(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return A_STAR_ERROR_16;

    std::uint32_t p = this->map[px][py];

    return static_cast<std::uint16_t>((p & A_STAR_FCOST_MASK) >> 16);
}

std::uint16_t A_star::_getgcost(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return A_STAR_ERROR_16;

    std::uint32_t p = this->map[px][py];

    return static_cast<std::uint16_t>((p & A_STAR_GCOST_MASK) >> 1);
}

void A_star::_setfcost(std::uint32_t px, std::uint32_t py, std::uint16_t f_cost)
{
    if (!_check_coords(px, py))
        return;

    std::uint32_t p = this->map[px][py];

    p = (p & A_STAR_FCOST_MASK_NEGATE) | static_cast<std::uint32_t>(f_cost << 16);

    this->map[px][py] = p;
}

void A_star::_setgcost(std::uint32_t px, std::uint32_t py, std::uint16_t g_cost)
{
    if (!_check_coords(px, py))
        return;

    std::uint32_t p = this->map[px][py];

    p = (p & A_STAR_GCOST_MASK_NEGATE) | static_cast<std::uint32_t>(g_cost << 1);

    this->map[px][py] = p;
}

/* START Min binary heap functions */

void A_star::_exchange(std::uint32_t i0, std::uint32_t i1)
{
    std::uint32_t tempx, tempy;
    tempx = this->px[i0];
    tempy = this->py[i0];

    this->px[i0] = this->px[i1];
    this->py[i0] = this->py[i1];

    this->px[i1] = tempx;
    this->py[i1] = tempy;
}

void A_star::_swim_cl(std::uint32_t pi)
{
    if (pi >= this->plc)
        return;

    std::uint32_t x = this->pxc[pi];
    std::uint32_t y = this->pyc[pi];
    uint16_t cost = _getfcost(x, y);

    std::uint32_t parent = (pi - 1) / 2;

    while (parent > 0)
    {
        if (cost > _getfcost(this->pxc[parent], this->pyc[parent]))
            break;

        _exchange(pi, parent);
        pi = parent;
        parent = (pi - 1) / 2;
    }
}

void A_star::_swim(std::uint32_t pi)
{
    if (pi >= this->pl)
        return;

    std::uint32_t x = this->px[pi];
    std::uint32_t y = this->py[pi];
    uint16_t cost = _getfcost(x, y);

    std::uint32_t parent = (pi - 1) / 2;

    while (parent > 0)
    {
        if (cost > _getfcost(this->px[parent], this->py[parent]))
            break;

        _exchange(pi, parent);
        pi = parent;
        parent = (pi - 1) / 2;
    }
}

void A_star::_sink_cl(std::uint32_t pi)
{
    if (pi >= this->plc)
        return;

    std::uint32_t x = this->pxc[pi];
    std::uint32_t y = this->pyc[pi];
    uint16_t cost = _getfcost(x, y);

    std::uint32_t left_child = 2 * pi + 1;
    std::uint32_t right_child = 2 * pi + 2;

    while (left_child < this->plc)
    {
        if (right_child >= this->plc)
        {
            if (cost > _getfcost(this->pxc[left_child], this->pyc[left_child]))
                _exchange(pi, left_child);
            pi = left_child;
            break;
        }

        uint16_t left_cost = _getfcost(this->pxc[left_child], this->pyc[left_child]);
        uint16_t right_cost = _getfcost(this->pxc[right_child], this->pyc[right_child]);

        if (left_cost < right_cost)
        {
            if (cost > left_cost)
            {
                _exchange(pi, left_child);
                pi = left_child;
            }
            else
                break;
        }
        else
        {
            if (cost > right_cost)
            {
                _exchange(pi, right_child);
                pi = right_child;
            }
            else
                break;
        }
        left_child = 2 * pi + 1;
        right_child = 2 * pi + 2;
    }
}

void A_star::_sink(std::uint32_t pi)
{
    if (pi >= this->pl)
        return;

    std::uint32_t x = this->px[pi];
    std::uint32_t y = this->py[pi];
    uint16_t cost = _getfcost(x, y);

    std::uint32_t left_child = 2 * pi + 1;
    std::uint32_t right_child = 2 * pi + 2;

    while (left_child < this->pl)
    {
        if (right_child >= this->pl)
        {
            if (cost > _getfcost(this->px[left_child], this->py[left_child]))
                _exchange(pi, left_child);
            pi = left_child;
            break;
        }

        uint16_t left_cost = _getfcost(this->px[left_child], this->py[left_child]);
        uint16_t right_cost = _getfcost(this->px[right_child], this->py[right_child]);

        if (left_cost < right_cost)
        {
            if (cost > left_cost)
            {
                _exchange(pi, left_child);
                pi = left_child;
            }
            else
                break;
        }
        else
        {
            if (cost > right_cost)
            {
                _exchange(pi, right_child);
                pi = right_child;
            }
            else
                break;
        }
        left_child = 2 * pi + 1;
        right_child = 2 * pi + 2;
    }
}

void A_star::_add(std::uint32_t x, std::uint32_t y)
{
    if (!_check_coords(x, y))
    {
        cout_warn("_add", "could not add, bad coordinates");
        return;
    }

    if (this->pl >= this->ps)
    {
        cout_warn("_add", "could not add, list size too tiny");
        return;
    }

    this->px[this->pl] = x;
    this->py[this->pl] = y;
    _swim(this->pl);
    this->pl++;
}

std::uint32_t A_star::_remove(std::uint32_t pi)
{
    if (this->pl <= 0)
    {
        cout_warn("_remove", "could not remove, empty list");
        return A_STAR_ERROR_32;
    }

    if (pi < 0 || pi > this->pl)
    {
        cout_warn("_remove", "could not remove, bad index");
        return A_STAR_ERROR_32;
    }

    _exchange(pi, this->pl);
    this->pl--;

    _sink(pi);
    return _getfcost(this->px[this->pl], this->py[this->pl]);
}

void A_star::_add_cl(std::uint32_t x, std::uint32_t y)
{
    if (this->plc >= this->ps)
    {
        cout_warn("_add", "could not add, list size too tiny");
        return;
    }

    this->pxc[this->plc] = x;
    this->pyc[this->plc] = y;
    _swim_cl(this->plc);
    this->plc++;
}

std::uint32_t A_star::_remove_cl(std::uint32_t pi)
{
    if (this->plc <= 0)
    {
        cout_warn("_remove", "could not remove, empty list");
        return A_STAR_ERROR_32;
    }

    if (pi < 0 || pi > this->plc)
    {
        cout_warn("_remove", "could not remove, bad index");
        return A_STAR_ERROR_32;
    }

    _exchange(pi, this->plc);
    this->pxc[this->plc] = 0;
    this->pyc[this->plc] = 0;
    this->plc--;

    _sink_cl(pi);
    return _getfcost(this->pxc[this->pl], this->pyc[this->pl]);
}

/* END Min binary heap functions */
