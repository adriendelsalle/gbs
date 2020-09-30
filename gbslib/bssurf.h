#pragma once
#include <gbslib/basisfunctions.h>
#include <gbslib/knotsfunctions.h>
#include <gbslib/bscurve.h>
#include <vector>
#include <array>
#include <algorithm>
namespace gbs
{
    /**
     * @brief Géneral BSpline surface class, any kind of precision, space dimension with rational definition capability
     * 
     * @tparam T 
     * @tparam dim 
     */
    template <typename T, size_t dim>
    class BSSurface
    {
        bool m_rational;
        size_t m_degU,m_degV;
        std::vector< std::array<T, dim> > m_poles;
        std::vector<T> m_weights;
        std::vector<T> m_knotsFlatsU;
        std::vector<T> m_knotsFlatsV;

    public:
    /**
     * @brief Construct a new BSSurface object, non rational definition
     * 
     * @param poles 
     * @param knots_flatsU 
     * @param knots_flatsV 
     * @param degU 
     * @param degV 
     */
        BSSurface(const std::vector<std::array<T, dim>> &poles,
                const std::vector<T> &knots_flatsU,
                const std::vector<T> &knots_flatsV,
                size_t degU,
                size_t degV
                ) : m_poles(poles),
                              m_knotsFlatsU(knots_flatsU),
                              m_knotsFlatsV(knots_flatsV),
                              m_degU(degU),
                              m_degV(degV),
                              m_rational(false)

        {
        }
    /**
     * @brief Construct a new BSSurface object, rational definition
     * 
     * @param poles 
     * @param weights 
     * @param knots_flatsU 
     * @param knots_flatsV 
     * @param degU 
     * @param degV 
     */
        BSSurface(const std::vector<std::array<T, dim>> &poles,
                  const std::vector<T> &weights,
                  const std::vector<T> &knots_flatsU,
                  const std::vector<T> &knots_flatsV,
                  size_t degU,
                  size_t degV) : m_poles(merge_weights(poles, weights)),
                                 m_knotsFlatsU(knots_flatsU),
                                 m_knotsFlatsV(knots_flatsV),
                                 m_degU(degU),
                                 m_degV(degV),
                                 m_rational(true)

        {
        }
    /**
     * @brief  Non rational curve evaluation
     * 
     * @param u  : u parameter on surface
     * @param v  : v parameter on surface
     * @param du : u derivative order
     * @param dv : v derivative order
     * @return std::array<T, dim> const 
     */
        auto value(T u, T v, size_t du = 0, size_t dv = 0) -> std::array<T, dim> const
        {
            return gbs::eval_value_simple(u, v, m_knotsFlatsU, m_knotsFlatsV, m_poles, m_degU, m_degV, du, dv);
        }
    /**
     * @brief Rational surface evaluation
     * 
     * @param u 
     * @param v 
     * @return std::array<T, dim - 1> const 
     */
        auto valueRational(T u, T v) -> std::array<T, dim - 1> const
        {
            return weight_projection(value(u, v, 0, 0));
        }
        // auto valueRational(T u, T v, size_t du = 0, size_t dv = 0) -> std::array<T, dim - 1> const
        // {
        //     return weight_projection(value(u, v, du, dv));
        // }

        auto degreeU() const noexcept -> size_t
        {
            return m_degU;
        }

        auto degreeV() const noexcept -> size_t
        {
            return m_degV;
        }

        auto knotsFlatsU() const noexcept -> const std::vector<T> &
        {
            return m_knotsFlatsU;
        }

        auto knotsFlatsV() const noexcept -> const std::vector<T> &
        {
            return m_knotsFlatsV;
        }

        // auto insertKnotU(T u, size_t m = 1) -> void //Fail saife, i.e. if fails, curve stays in precedent state
        // {
        //     for (auto i = 0; i < m; i++)
        //         insert_knot(u, m_deg, m_knotsFlatsU, m_poles);
        // }

        // auto removeKnot(T u, T tol, size_t m = 1) -> void //Fail saife, i.e. if fails, curve stays in precedent state
        // {
        //     for (auto i = 0; i < m; i++)
        //         remove_knot(u, m_deg, m_knotsFlats, m_poles, tol);
        // }

        auto poles() const noexcept -> const std::vector<std::array<T, dim>> &
        {
            return m_poles;
        }

        auto nPolesU() const noexcept -> size_t
        {
            return m_knotsFlatsU.size() - m_degU - 1;
        }

        auto nPolesV() const noexcept -> size_t
        {
            return m_knotsFlatsV.size() - m_degV - 1;
        }

        auto isRational() const noexcept -> bool 
        {
            return m_rational;
        }
    };

    // template <typename T, size_t dim>
    // class NURBSSurface : public BSSurface<T,dim+1>
    // {
    //     public:
    //     NURBSSurface(const std::vector< std::array<T, dim+1> > &poles,
    //             const std::vector<T> &knots_flatsU,
    //             const std::vector<T> &knots_flatsV,
    //             size_t degU,
    //             size_t degV
    //             ) : BSSurface<T,dim+1>(poles,knots_flatsU,knots_flatsV,degU,degV)
    //     {
    //     }

    //     virtual auto value(T u, T v, size_t du = 0, size_t dv = 0) /*-> std::array<T, dim>*/ const //override
    //     {
    //         auto p = BSSurface<T,dim+1>::value(u, v, du, dv);
    //         std::array<T, dim> r;
    //         std::transform(p.begin(), std::next(p.end(), -1), r.begin(), [&p](const auto &p_) { return p_ / p.back(); });
    //         return r;
    //     }
    // };

} // namespace gbs