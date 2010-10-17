//
// Copyright 2010 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef INCLUDED_UHD_USRP_SINGLE_USRP_HPP
#define INCLUDED_UHD_USRP_SINGLE_USRP_HPP

#include <uhd/config.hpp>
#include <uhd/device.hpp>
#include <uhd/types/ranges.hpp>
#include <uhd/types/stream_cmd.hpp>
#include <uhd/types/clock_config.hpp>
#include <uhd/types/tune_result.hpp>
#include <uhd/usrp/subdev_spec.hpp>
#include <uhd/usrp/dboard_iface.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <vector>

namespace uhd{ namespace usrp{

/*!
 * The single-USRP device class:
 * A single-USRP facilitates ease-of-use for most use-case scenarios.
 * The wrapper provides convenience functions to tune the devices
 * as well as to set the dboard gains, antennas, and other properties.
 * This wrapper supports multi-channel configurations per motherboard.
 */
class UHD_API single_usrp : boost::noncopyable{
public:
    typedef boost::shared_ptr<single_usrp> sptr;

    /*!
     * Make a new single usrp from the device address.
     * \param dev_addr the device address
     * \return a new single usrp object
     */
    static sptr make(const device_addr_t &dev_addr);

    /*!
     * Get the underlying device object.
     * This is needed to get access to the streaming API and properties.
     * \return the device object within this single usrp
     */
    virtual device::sptr get_device(void) = 0;

    /*******************************************************************
     * Mboard methods
     ******************************************************************/
    /*!
     * Get a printable summary for this USRP configuration.
     * \return a printable string
     */
    virtual std::string get_pp_string(void) = 0;

    /*!
     * Get canonical name for this USRP motherboard.
     * \return a string representing the name
     */
    virtual std::string get_mboard_name(void) = 0;

    /*!
     * Gets the current time in the usrp time registers.
     * \return a timespec representing current usrp time
     */
    virtual time_spec_t get_time_now(void) = 0;

    /*!
     * Sets the time registers on the usrp immediately.
     * \param time_spec the time to latch into the usrp device
     */
    virtual void set_time_now(const time_spec_t &time_spec) = 0;

    /*!
     * Set the time registers on the usrp at the next pps tick.
     * The values will not be latched in until the pulse occurs.
     * It is recommended that the user sleep(1) after calling to ensure
     * that the time registers will be in a known state prior to use.
     *
     * Note: Because this call sets the time on the "next" pps,
     * the seconds in the time spec should be current seconds + 1.
     *
     * \param time_spec the time to latch into the usrp device
     */
    virtual void set_time_next_pps(const time_spec_t &time_spec) = 0;

    /*!
     * Issue a stream command to the usrp device.
     * This tells the usrp to send samples into the host.
     * See the documentation for stream_cmd_t for more info.
     * \param stream_cmd the stream command to issue
     */
    virtual void issue_stream_cmd(const stream_cmd_t &stream_cmd) = 0;

    /*!
     * Set the clock configuration for the usrp device.
     * This tells the usrp how to get a 10Mhz reference and PPS clock.
     * See the documentation for clock_config_t for more info.
     * \param clock_config the clock configuration to set
     */
    virtual void set_clock_config(const clock_config_t &clock_config) = 0;

    /*******************************************************************
     * RX methods
     ******************************************************************/
    /*!
     * Set the RX subdevice specification:
     * The subdev spec maps a physical part of a daughter-board to a channel number.
     * Set the subdev spec before calling into any methods with a channel number.
     */
    virtual void set_rx_subdev_spec(const uhd::usrp::subdev_spec_t &spec) = 0;
    virtual uhd::usrp::subdev_spec_t get_rx_subdev_spec(void) = 0;

    virtual std::string get_rx_subdev_name(size_t chan = 0) = 0;

    virtual void set_rx_rate(double rate) = 0;
    virtual double get_rx_rate(void) = 0;

    virtual tune_result_t set_rx_freq(double freq, size_t chan = 0) = 0;
    virtual tune_result_t set_rx_freq(double freq, double lo_off, size_t chan = 0) = 0;
    virtual double get_rx_freq(size_t chan = 0) = 0;
    virtual freq_range_t get_rx_freq_range(size_t chan = 0) = 0;

    virtual void set_rx_gain(float gain, size_t chan = 0) = 0;
    virtual float get_rx_gain(size_t chan = 0) = 0;
    virtual gain_range_t get_rx_gain_range(size_t chan = 0) = 0;

    virtual void set_rx_antenna(const std::string &ant, size_t chan = 0) = 0;
    virtual std::string get_rx_antenna(size_t chan = 0) = 0;
    virtual std::vector<std::string> get_rx_antennas(size_t chan = 0) = 0;

    virtual bool get_rx_lo_locked(size_t chan = 0) = 0;

    /*!
     * Read the RSSI value from a usrp device.
     * Or throw if the dboard does not support an RSSI readback.
     * \return the rssi in dB
     */
    virtual float read_rssi(size_t chan = 0) = 0;

    virtual dboard_iface::sptr get_rx_dboard_iface(size_t chan = 0) = 0;
    
    virtual void set_rx_bandwidth(float bandwidth, size_t chan = 0) = 0;

    /*******************************************************************
     * TX methods
     ******************************************************************/
    /*!
     * Set the TX subdevice specification:
     * The subdev spec maps a physical part of a daughter-board to a channel number.
     * Set the subdev spec before calling into any methods with a channel number.
     */
    virtual void set_tx_subdev_spec(const uhd::usrp::subdev_spec_t &spec) = 0;
    virtual uhd::usrp::subdev_spec_t get_tx_subdev_spec(void) = 0;

    virtual std::string get_tx_subdev_name(size_t chan = 0) = 0;

    virtual void set_tx_rate(double rate) = 0;
    virtual double get_tx_rate(void) = 0;

    virtual tune_result_t set_tx_freq(double freq, size_t chan = 0) = 0;
    virtual tune_result_t set_tx_freq(double freq, double lo_off, size_t chan = 0) = 0;
    virtual double get_tx_freq(size_t chan = 0) = 0;
    virtual freq_range_t get_tx_freq_range(size_t chan = 0) = 0;

    virtual void set_tx_gain(float gain, size_t chan = 0) = 0;
    virtual float get_tx_gain(size_t chan = 0) = 0;
    virtual gain_range_t get_tx_gain_range(size_t chan = 0) = 0;

    virtual void set_tx_antenna(const std::string &ant, size_t chan = 0) = 0;
    virtual std::string get_tx_antenna(size_t chan = 0) = 0;
    virtual std::vector<std::string> get_tx_antennas(size_t chan = 0) = 0;

    virtual bool get_tx_lo_locked(size_t chan = 0) = 0;

    virtual dboard_iface::sptr get_tx_dboard_iface(size_t chan = 0) = 0;
};

}}

#endif /* INCLUDED_UHD_USRP_SINGLE_USRP_HPP */
