#include "a_star.hh"
#include "ioutils.hh"

#include <cstdlib>
#include <iostream>
#include <cmath>

void A_star::toggletile(std::uint32_t px, std::uint32_t py, bool tile_state)
{
    if (!_check_map())
        return;
    if (!_check_coords(px, py))
        return;

    this->map[px][py] = tile_state;
}

void A_star::run(std::uint32_t sx, std::uint32_t sy, std::uint32_t tx, std::uint32_t ty)
{
    
}

bool A_star::_check_map()
{
    if (this->map == nullptr)
    {
        cout_err("_getcost", "the 'map' variable is null");
        return false;
    }

    return true;
}

bool A_star::_check_coords(std::uint32_t px, std::uint32_t py)
{
    if (!_check_map())
    {
        return false;
    }

    else if (this->map[px] == nullptr)
    {
        cout_err("_getcost", "the row to lookup is null (mem corruption?)");
        return false;
    }

    if (px * py >= this->xs * this->ys)
    {
        cout_err("_getcost", "indices out of bounds");
        return false;
    }

    return true;
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
    this->ps = xs * ys;
    this->pl = 0;
}

void A_star::_freepnt(std::uint32_t *ptr)
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

    return (this->map[px][py] & A_STAR_STATE_MASK_NEGATE) == 0xFF;
}

std::uint16_t A_star::_getcost(std::uint32_t px, std::uint32_t py)
{
    if (!_check_coords(px, py))
        return A_STAR_ERROR_16;

    std::uint32_t p = this->map[px][py];

    return static_cast<std::uint16_t>(p >> 16);
}

void A_star::_setcost(std::uint32_t px, std::uint32_t py, std::uint16_t f_cost)
{
    if (!_check_coords(px, py))
        return;

    std::uint32_t p = this->map[px][py];

    p = (p & A_STAR_FCOST_MASK_NEGATE) | static_cast<std::uint32_t>(f_cost << 16);

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

void A_star::_swim(std::uint32_t pi)
{
    if (pi >= this->ps)
        return;

    std::uint32_t x = this->px[pi];
    std::uint32_t y = this->py[pi];
    uint16_t cost = _getcost(x, y);

    std::uint32_t parent = (pi - 1) / 2;

    while (parent > 0)
    {
        if (cost > _getcost(this->px[parent], this->py[parent]))
            break;

        _exchange(pi, parent);
        pi = parent;
        parent = (pi - 1) / 2;
    }
}

void A_star::_sink(std::uint32_t pi)
{
    if (pi >= this->ps)
        return;

    std::uint32_t x = this->px[pi];
    std::uint32_t y = this->py[pi];
    uint16_t cost = _getcost(x, y);

    std::uint32_t left_child = 2 * pi + 1;
    std::uint32_t right_child = 2 * pi + 2;

    while (left_child < this->ps)
    {
        if (right_child >= this->ps)
        {
            if (cost > _getcost(this->px[left_child], this->py[left_child]))
                _exchange(pi, left_child);
            pi = left_child;
            break;
        }

        uint16_t left_cost = _getcost(this->px[left_child], this->py[left_child]);
        uint16_t right_cost = _getcost(this->px[right_child], this->py[right_child]);

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
        return A_STAR_ERROR;
    }

    if (pi < 0 || pi > this->pl)
    {
        cout_warn("_remove", "could not remove, bad index");
        return A_STAR_ERROR;
    }

    _exchange(pi, this->pl);
    this->px[this->pl] = 0;
    this->py[this->pl] = 0;
    this->pl--;

    _sink(pi);
}

/* END Min binary heap functions */
